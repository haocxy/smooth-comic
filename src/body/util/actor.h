#pragma once

#include <functional>
#include <typeindex>
#include <memory>
#include <map>

#include "thread.h"


namespace myapp::actor {

class Actor;
class Action;
class ActionResult;


// 消息
// 消息是 actor 模式中在不同 Actor 中传递信息的实体
class Event {
public:
    Event() {}

    virtual ~Event() {}

    virtual void handle(Actor &receiver) {};
};

class Result {
public:
    virtual ~Result() {}
};

using ActionCallback = std::function<void(Result &)>;

class Actor {
public:

    Actor() : handle_(std::make_shared<Handle>(*this)) {}

    virtual ~Actor() {}

public: // 这部分是为业务逻辑提供的
    void sendTo(Actor &receiver, std::unique_ptr<Action> action, ActionCallback &&cb);

    template <typename ResultType>
    void sendTo(Actor &receiver, std::unique_ptr<Action> action, std::function<void(ResultType &)> &&cb) {
        sendTo(receiver, std::move(action), [cb = std::move(cb)](Result &result) mutable {
            cb(static_cast<ResultType &>(result));
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

    void post(std::unique_ptr<Event> e) {
        eventQueue_.push(std::move(e));
    }

    // 这个函数是子类处理消息的总入口，固定了 Actor 处理消息的框架
    // 子类负责则在适当的时候调用这个函数
    void handleEvent(Event &e) {
        e.handle(*this);
    }

    virtual std::unique_ptr<Result> dispatch(const Action &a) = 0;

protected:

    BlockQueue<std::unique_ptr<Event>> eventQueue_;

    std::atomic_bool stopped_{ false };

private:
    void handleAction(Action &action);
    void handleActionResult(ActionResult &actionResult);

private:
    // Handle 类的核心逻辑就是配合 std::shared_ptr 实现的，必须由 shared_ptr 管理
    std::shared_ptr<Handle> handle_;

    friend class Action;
    friend class ActionResult;
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



// 动作
// 和消息（Event）不同，消息通常是单向的，而动作是需要回调的
class Action : public Event {
public:
    using Callback = ActionCallback;

    Action() {}

    Action(std::weak_ptr<Actor::Handle> senderHandle, Callback &&callback)
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

    virtual ~Action() {}

protected:
    virtual void callback() {}

private:
    virtual void handle(Actor &receiver) override {
        receiver.handleAction(*this);
    }

private:
    std::weak_ptr<Actor::Handle> sender_{};
    Callback callback_;

    friend class Actor;
};

class ActionResult : public Event {
public:
    using Callback = Action::Callback;

    ActionResult(Callback &&callback, std::unique_ptr<Result> &&result)
        : callback_(std::move(callback)), result_(std::move(result)) {}

    virtual void handle(Actor &receiver) override {
        receiver.handleActionResult(*this);
    }

    virtual ~ActionResult() {}

private:
    Callback callback_;
    std::unique_ptr<Result> result_;

    friend class Actor;
};

}
