#pragma once

#include <functional>
#include <typeindex>
#include <memory>
#include <map>

#include "thread.h"


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

    virtual void handle(Actor &receiver) {}

private:



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

    Actor() : handle_(std::make_shared<Handle>(*this)) {}

    virtual ~Actor() {}

public:
    template <typename RequestType>
    void sendTo(Actor &receiver, std::unique_ptr<RequestType> req, std::function<void(typename RequestType::Response &)> &&cb) {
        doSendTo(receiver, std::move(req), [cb = std::move(cb)](Response &resp) mutable {
            cb(static_cast<typename RequestType::Response &>(resp));
        });
    }

protected:
    virtual void onActorStarted() {}

    virtual void onActorStopped() {}

protected: // 这部分是用于实现 Actor 框架内部逻辑的代码

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

    void post(std::unique_ptr<detail::Event> e) {
        eventQueue_.push(std::move(e));
    }

    // 这个函数是子类处理消息的总入口，固定了 Actor 处理消息的框架
    // 子类负责则在适当的时候调用这个函数
    void handleEvent(detail::Event &e) {
        e.handle(*this);
    }

    virtual std::unique_ptr<Response> dispatch(const Request &a) {
        return nullptr;
    }

protected:

    BlockQueue<std::unique_ptr<detail::Event>> eventQueue_;

    std::atomic_bool stopped_{ false };

private:
    void doSendTo(Actor &receiver, std::unique_ptr<Request> req, ActionCallback &&cb);

    void handleRequest(Request &action);

    void handleRequestCallbackEvent(detail::RequestCallbackEvent &actionResult);

private:
    // Handle 类的核心逻辑就是配合 std::shared_ptr 实现的，必须由 shared_ptr 管理
    std::shared_ptr<Handle> handle_;

    friend class Request;
    friend class detail::RequestCallbackEvent;
};


class ThreadedActor : public Actor {
public:
    ThreadedActor();

    virtual ~ThreadedActor();

private:
    void loop();

private:
    std::jthread recvThread_;
};


class Request : public detail::Event {
public:
    using Callback = ActionCallback;

    Request() {}

    Request(std::weak_ptr<Actor::Handle> senderHandle, Callback &&callback)
        : sender_(senderHandle)
        , callback_(std::move(callback)) {}

    template <typename T>
    const T *tryAs() const {
        return dynamic_cast<const T *>(this);
    }

    template <typename T>
    T *tryAs() {
        return dynamic_cast<T *>(this);
    }

    virtual ~Request() {}

protected:
    virtual void callback() {}

private:
    virtual void handle(Actor &receiver) override {
        receiver.handleRequest(*this);
    }

private:
    std::weak_ptr<Actor::Handle> sender_{};
    Callback callback_;

    friend class Actor;
};


namespace detail {

class RequestCallbackEvent : public detail::Event {
public:
    using Callback = Request::Callback;

    RequestCallbackEvent(Callback &&callback, std::unique_ptr<Response> &&resp)
        : callback_(std::move(callback)), resp_(std::move(resp)) {}

    virtual void handle(Actor &receiver) override {
        receiver.handleRequestCallbackEvent(*this);
    }

    virtual ~RequestCallbackEvent() {}

private:
    Callback callback_;
    std::unique_ptr<Response> resp_;

    friend class Actor;
};

} // namespace detail


class Message : public detail::Event {
public:
    // TODO
};

}
