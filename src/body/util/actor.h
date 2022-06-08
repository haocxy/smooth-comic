#pragma once

#include <functional>
#include <typeindex>
#include <memory>
#include <map>

#include "core/thread.h"


namespace myapp::actor {

class Actor;

// 请求：从 actor A 发送到指定的 actor B，actor B 执行完成后一定发回配套的响应
class Request;

// 消息：从 actor A 发送到指定的 actor B，无配套响应
class Message;

// 通知：actor A 发送到若干通知订阅者，发送时不关心有哪些订阅者
class Notice;

// 任务：actor A 发送到指定的 actor B，可以取消执行，可以回调多次
class Task;

namespace detail {
class RunInActorEvent;
class RequestCallbackEvent;
class AddListenerMessage;
}

namespace detail {

// 消息
// 消息是 actor 模式中在不同 Actor 中传递信息的实体
// 该类是用于 actor 模块内部实现的类，外部用户请使用 Message 或 Request
class Event {
public:
    Event() {}

    virtual ~Event() {}

    template <typename T>
    operator const T *() const {
        return dynamic_cast<const T *>(this);
    }

    template <typename T>
    operator T *() {
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

    Actor();

    virtual ~Actor();

    virtual void setActorName(const std::string_view &name) {
        actorName_ = name;
    }

    virtual const std::string &actorName() const {
        return actorName_;
    }

    virtual void post(std::unique_ptr<detail::Event> &&e) = 0;

    void post(std::function<void()> &&action);

    template <typename RequestType>
    void sendTo(Actor &receiver, std::unique_ptr<RequestType> req, std::function<void(typename RequestType::Response &)> &&cb) {
        doSendTo(receiver, std::move(req), [cb = std::move(cb)](Response &resp) mutable {
            cb(static_cast<typename RequestType::Response &>(resp));
        });
    }

    template <typename MessageType>
    void sendTo(Actor &receiver, std::unique_ptr<MessageType> &&msg) {
        msg->sender_ = this->handle_;
        receiver.post(std::move(msg));
    }

    template <typename MessageType>
    void sendTo(Actor &receiver, MessageType *&&msg) {
        std::unique_ptr<MessageType> uptr(msg);
        msg = nullptr;
        sendTo(receiver, std::move(uptr));
    }

    template <typename MessageType>
    void sendTo(std::weak_ptr<Handle> receiver, std::unique_ptr<MessageType> &&msg) {
        if (std::shared_ptr<Handle> handle = receiver.lock()) {
            sendTo(handle->actor(), std::move(msg));
        }
    }

    template <typename NoticeType>
    void listen(Actor &receiver) {
        doAddListenerTo(receiver, typeid(NoticeType));
    }

    template <typename NoticeType>
    void notify(std::unique_ptr<NoticeType> &&notice) {
        doNotify(typeid(NoticeType), std::move(notice));
    }

protected:
    virtual void onActorStarted() {}

    virtual void onActorStopped() {}

    virtual std::unique_ptr<Response> onRequest(Request &a) { return nullptr; }

    virtual void onMessage(Message &msg) {}

    virtual void onNotice(Notice &notice) {}

    virtual void onTask(Task &task) {}

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

    void handleNotice(Notice &notice) {
        onNotice(notice);
    }

    void handleTask(Task &task) {
        onTask(task);
    }

    void handleRunInActor(detail::RunInActorEvent &runInActorEvent);

    void handleAddListenerMessage(detail::AddListenerMessage &msg);

    bool hasListener(std::vector<std::weak_ptr<Handle>> &vec, std::shared_ptr<Handle> handle) {
        for (std::weak_ptr<Handle> item : vec) {
            if (item.lock() == handle) {
                return true;
            }
        }
        return false;
    }

    void doAddListenerTo(Actor &receiver, const std::type_index &noticeType);

    void doNotify(std::type_index type, std::unique_ptr<Notice> &&notice);

private:

    // Handle 类的核心逻辑就是配合 std::shared_ptr 实现的，必须由 shared_ptr 管理
    std::shared_ptr<Handle> handle_;

    std::string actorName_;

    struct CmpWeakPtrToHandle {
        bool operator()(const std::weak_ptr<Handle> &a, const std::weak_ptr<Handle> &b) const {
            return a.lock().get() < b.lock().get();
        }
    };

    using HandleSet = std::set<std::weak_ptr<Handle>, CmpWeakPtrToHandle>;

    std::map<std::type_index, HandleSet> listeners_;

    friend class Request;
    friend class Message;
    friend class Notice;
    friend class Task;

    friend class detail::RunInActorEvent;
    friend class detail::RequestCallbackEvent;
    friend class detail::AddListenerMessage;
};

using WeakHandle = std::weak_ptr<Actor::Handle>;


class ThreadedActor : public Actor {
public:
    ThreadedActor();

    virtual ~ThreadedActor();

    virtual void setActorName(const std::string_view &name) override {
        Actor::setActorName(name);
        nameModified_ = true;
    }

    virtual void post(std::unique_ptr<detail::Event> &&e) override {
        eventQueue_.push(std::move(e));
    }

    // 停止消息处理，
    // 子类必须在析构函数中显式调用，以确保在子类的组件释放前停止消息处理
    void stopEventHandle();

protected:
    std::atomic_bool stopped_{ false };

private:
    void loop();

    void updateThreadName();

private:
    BlockQueue<std::unique_ptr<detail::Event>> eventQueue_;
    std::jthread recvThread_;
    std::atomic_bool nameModified_{ false };
};

namespace detail {

class SenderAwaredEvent : public Event {
public:
    SenderAwaredEvent() {}

    SenderAwaredEvent(const SenderAwaredEvent &other)
        : sender_(other.sender_) {}

    virtual ~SenderAwaredEvent() {}

    std::weak_ptr<Actor::Handle> sender() {
        return sender_.lock();
    }

    void setSender(std::weak_ptr<Actor::Handle> handle) {
        sender_ = handle;
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

class RunInActorEvent : public detail::Event {
public:
    RunInActorEvent(std::function<void()> &&runBody)
        : runBody_(std::move(runBody)) {}

    virtual ~RunInActorEvent() {}

protected:
    virtual void handle(Actor &receiver) override {
        receiver.handleRunInActor(*this);
    }

private:
    std::function<void()> runBody_;

    friend class Actor;
};

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


namespace detail {

class AddListenerMessage : public Message {
public:

    AddListenerMessage(const std::type_index &noticeType) : noticeType_(noticeType) {}

    virtual ~AddListenerMessage() {}

    const std::type_index &noticeType() const {
        return noticeType_;
    }

protected:
    virtual void handle(Actor &receiver) override {
        receiver.handleAddListenerMessage(*this);
    }

private:
    std::type_index noticeType_;

    friend class Actor;
};

}


class Notice : public detail::SenderAwaredEvent {
public:
    Notice() {}

    Notice(const Notice &other)
        : detail::SenderAwaredEvent(*this) {}

    virtual ~Notice() {}

    virtual Notice *clone() const = 0;

protected:
    virtual void handle(Actor &receiver) override {
        receiver.handleNotice(*this);
    }
};

// 任务
// 用于支持需要较长时间完成处理的逻辑，支持回调处理结果和中途取消任务
class Task : public detail::SenderAwaredEvent {
private:

    // 由发送方和接收方共享的状态
    // 
    // 功能：
    // 1：检查 Task 是否存活
    // 2：在双方传递信息，例如实现取消任务、回调等
    // 
    // 注意：
    // 不要直接暴露这个类到 Task 类之外，外部应使用 Task 和 TaskController
    class ControllBlock : public std::enable_shared_from_this<ControllBlock> {
    public:
        ControllBlock(Task &task) : task_(task) {}

        bool isCanceled() const {
            return canceled_;
        }

    private:
        Task &task_;
        std::atomic_bool canceled_{ false };
    };

public:

    // 由发送方持有，用于注册回调或取消任务等
    class Controller {
    public:
        Controller(std::weak_ptr<ControllBlock> controllBlock)
            : controllBlock_(controllBlock) {}

    private:
        std::weak_ptr<ControllBlock> controllBlock_;
    };

public:
    Task()
        : controllBlock_(std::make_shared<ControllBlock>(*this)) {}

    Task(const Task &) = delete;

    Task &operator=(const Task &) = delete;

    // 构造一个 Controller 对象
    Controller createController() const {
        return Controller(controllBlock_);
    }

    // 检查这个任务是否被取消
    // 处理任务的逻辑应该按照合适的频率检查这个标记，以支持取消任务
    bool isCanceled() const {
        return controllBlock_->isCanceled();
    }

protected:
    virtual void handle(Actor &receiver) override {
        receiver.handleTask(*this);
    }

private:
    std::shared_ptr<ControllBlock> controllBlock_;
};

}
