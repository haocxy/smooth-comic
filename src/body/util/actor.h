#pragma once

#include <functional>
#include <typeindex>
#include <memory>
#include <map>

#include "core/thread.h"


namespace myapp::actor {

class Actor;

class Request;

namespace detail {
class RequestCallbackEvent;
}

class Message;


namespace detail {

// 消息
// 消息是 actor 模式中在不同 Actor 中传递信息的实体
// 该类是用于 actor 模块内部实现的类，外部用户请使用 Message 或 Request
class Event {
public:
    Event() {}

    virtual ~Event() {}

    template <typename T>
    const T *tryAs() const {
        return dynamic_cast<const T *>(this);
    }

    template <typename T>
    T *tryAs() {
        return dynamic_cast<T *>(this);
    }

protected:
    virtual void handle(Actor &receiver) {}

private:
    friend class Actor;
    friend class Request;
    friend class RequestCallbackEvent;
    friend class Message;
};

} // namespace detail


class Response {
public:
    virtual ~Response() {}
};

using ActionCallback = std::function<void(Response &)>;

class Actor {
public:

    // Actor 句柄
    // 用于检测所属的 Actor 的生命周期是否结束
    // 简化代码，通过这个类，就不需要特殊关注 Actor 类本身是如何构造的
    class Handle : public std::enable_shared_from_this<Handle> {
    public:
        Handle(Actor &actor) : actor_(actor) {}
        virtual ~Handle() {}
        Actor &actor() { return actor_; }
    private:
        Actor &actor_;
    };

    Actor() : handle_(std::make_shared<Handle>(*this)) {}

    virtual ~Actor() {}

    virtual void setActorName(const std::string_view &name) {
        actorName_ = name;
    }

    virtual const std::string &actorName() const {
        return actorName_;
    }

    virtual void post(std::unique_ptr<detail::Event> e) = 0;

    template <typename RequestType>
    void sendTo(Actor &receiver, std::unique_ptr<RequestType> req, std::function<void(typename RequestType::Response &)> &&cb) {
        doSendTo(receiver, std::move(req), [cb = std::move(cb)](Response &resp) mutable {
            cb(static_cast<typename RequestType::Response &>(resp));
        });
    }

    template <typename MessageType>
    void sendTo(Actor &receiver, std::unique_ptr<MessageType> msg) {
        msg->sender_ = this->handle_;
        receiver.post(std::move(msg));
    }

protected:
    virtual void onActorStarted() {}

    virtual void onActorStopped() {}

    virtual std::unique_ptr<Response> onRequest(Request &a) { return nullptr; }

    virtual void onMessage(Message &msg) {}

    // 这个函数是子类处理消息的总入口，固定了 Actor 处理消息的框架
    // 子类负责则在适当的时候调用这个函数
    void handleEvent(detail::Event &e) {
        e.handle(*this);
    }

private:
    void doSendTo(Actor &receiver, std::unique_ptr<Request> req, ActionCallback &&cb);

    void handleRequest(Request &action);

    void handleRequestCallbackEvent(detail::RequestCallbackEvent &actionResult);

    void handleMessage(Message &message) {
        onMessage(message);
    }


private:

    // Handle 类的核心逻辑就是配合 std::shared_ptr 实现的，必须由 shared_ptr 管理
    std::shared_ptr<Handle> handle_;

    std::string actorName_;

    friend class Request;
    friend class detail::RequestCallbackEvent;
    friend class Message;
};


class ThreadedActor : public Actor {
public:
    ThreadedActor();

    virtual ~ThreadedActor();

    virtual void setActorName(const std::string_view &name) override {
        Actor::setActorName(name);
        nameModified_ = true;
    }

    virtual void post(std::unique_ptr<detail::Event> e) override {
        eventQueue_.push(std::move(e));
    }

private:
    void loop();

    void updateThreadName();

private:
    BlockQueue<std::unique_ptr<detail::Event>> eventQueue_;
    std::atomic_bool stopped_{ false };
    std::jthread recvThread_;
    std::atomic_bool nameModified_{ false };
};

namespace detail {

class SenderAwaredEvent : public Event {
public:
    SenderAwaredEvent() {}

    virtual ~SenderAwaredEvent() {}

    std::shared_ptr<Actor::Handle> sender() {
        return sender_.lock();
    }

protected:
    std::weak_ptr<Actor::Handle> sender_;

    friend class Actor;
};

}

class Request : public detail::SenderAwaredEvent {
public:
    using Callback = ActionCallback;

    Request() {}

    virtual ~Request() {}

protected:
    virtual void handle(Actor &receiver) override {
        receiver.handleRequest(*this);
    }

private:
    Callback callback_;

    friend class Actor;
};


namespace detail {

class RequestCallbackEvent : public detail::Event {
public:
    using Callback = Request::Callback;

    RequestCallbackEvent(Callback &&callback, std::unique_ptr<Response> &&resp)
        : callback_(std::move(callback)), resp_(std::move(resp)) {}

    virtual ~RequestCallbackEvent() {}

protected:
    virtual void handle(Actor &receiver) override {
        receiver.handleRequestCallbackEvent(*this);
    }

private:
    Callback callback_;
    std::unique_ptr<Response> resp_;

    friend class Actor;
};

} // namespace detail


class Message : public detail::SenderAwaredEvent {
public:
    Message() {}

    virtual ~Message() {}

protected:
    virtual void handle(Actor &receiver) override {
        receiver.handleMessage(*this);
    }
};

}
