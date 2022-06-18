#pragma once

#include <type_traits>
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

template <typename T>
class EventHolder;

namespace detail {

// 消息
// 消息是 actor 模式中在不同 Actor 中传递信息的实体
// 该类是用于 actor 模块内部实现的类，外部用户请使用 Message 或 Request
class Event {
public:
    Event() {}

    virtual ~Event() {}

    Event(const Event &other) {
        *this = other;
    }

    Event &operator=(const Event &other) {
        return *this;
    }

    template <typename T>
    operator const T *() const {
        return dynamic_cast<const T *>(this);
    }

    template <typename T>
    operator T *() {
        return dynamic_cast<T *>(this);
    }

protected:
    virtual void handle(Actor &receiver, EventHolder<Event> &&holder) {}

private:
    friend class Actor;
    friend class Request;
    friend class RequestCallbackEvent;
    friend class Message;
};

} // namespace detail

template <typename T>
class EventHolder {
public:
    EventHolder() {}

    template <typename U>
    EventHolder(U *&&ptr)
        : ptr_(ptr) {
        ptr = nullptr;
    }

    template <typename U>
    EventHolder(const EventHolder<U> &) = delete;

    template <typename U>
    EventHolder(EventHolder<U> &&other) {
        *this = std::move(other);
    }

    template <typename U>
    EventHolder &operator=(const EventHolder<U> &) = delete;

    template <typename U>
    EventHolder &operator=(EventHolder<U> &&other) {
        if constexpr (std::is_base_of_v<T, U> || std::is_same_v<T, U>) {
            ptr_ = std::move(other.ptr_);
        } else {
            T *p = dynamic_cast<T *>(other.ptr_.get());
            if (p) {
                ptr_ = std::unique_ptr<T>(p);
                other.ptr_.release();
            }
        }
        return *this;
    }

    operator bool() const {
        return ptr_.operator bool();
    }

    T &operator*() const {
        return ptr_.operator*();
    }

    T *operator->() const {
        return ptr_.operator->();
    }

private:
    std::unique_ptr<T> ptr_;

    template <typename U>
    friend class EventHolder;
};

class Response {
public:
    Response() {}

    Response(const Response &other) {
        *this = other;
    }

    Response &operator=(const Response &other) {
        return *this;
    }

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

    void post(std::function<void()> &&action);

    virtual void post(EventHolder<detail::Event> &&e) = 0;

    template <typename EventType>
    void post(EventHolder<EventType> &&e) {
        post(EventHolder<detail::Event>(std::move(e)));
    }

    template <typename EventType>
    void post(EventType *&&e) {
        post(EventHolder<EventType>(std::move(e)));
    }

    

    template <typename RequestType>
    void requestTo(Actor &receiver, EventHolder<RequestType> &&req, std::function<void(typename RequestType::Response &)> &&cb) {
        doSendReqTo(receiver, std::move(req), [cb = std::move(cb)](Response &resp) mutable {
            cb(dynamic_cast<typename RequestType::Response &>(resp));
        });
    }

    template <typename RequestType>
    void requestTo(Actor &receiver, RequestType *&&req, std::function<void(typename RequestType::Response &)> &&cb) {
        requestTo<RequestType>(receiver, EventHolder<RequestType>(std::move(req)), std::move(cb));
    }

    void respondTo(EventHolder<Request> &&req, actor::EventHolder<Response> &&resp);

    template <typename MessageType>
    void sendTo(Actor &receiver, EventHolder<MessageType> &&msg) {
        msg->sender_ = this->handle_;
        receiver.post(std::move(msg));
    }

    template <typename MessageType>
    void sendTo(Actor &receiver, MessageType *&&msg) {
        sendTo(receiver, EventHolder<MessageType>(std::move(msg)));
    }

    template <typename MessageType>
    void sendTo(std::weak_ptr<Handle> receiver, EventHolder<MessageType> &&msg) {
        if (std::shared_ptr<Handle> handle = receiver.lock()) {
            sendTo(handle->actor(), std::move(msg));
        }
    }

    template <typename MessageType>
    void sendTo(std::weak_ptr<Handle> receiver, MessageType *&&msg) {
        sendTo(receiver, EventHolder<MessageType>(std::move(msg)));
    }

    template <typename NoticeType>
    void listen(Actor &receiver) {
        doAddListenerTo(receiver, typeid(NoticeType));
    }

    template <typename NoticeType>
    void notify(EventHolder<NoticeType> &&notice) {
        doNotify(typeid(NoticeType), std::move(notice));
    }

    template <typename NoticeType>
    void notify(NoticeType *&&notice) {
        notify(EventHolder<NoticeType>(std::move(notice)));
    }

protected:
    virtual void onActorStarted() {}

    virtual void onActorStopped() {}

    virtual void onRequest(EventHolder<Request> &&req) {}

    virtual void onMessage(EventHolder<Message> &&msg) {}

    virtual void onNotice(EventHolder<Notice> &&notice) {}

    virtual void onTask(EventHolder<Task> &&task) {}

    // 这个函数是子类处理消息的总入口，固定了 Actor 处理消息的框架
    // 子类负责则在适当的时候调用这个函数
    void handleEvent(EventHolder<detail::Event> &&e) {
        e->handle(*this, std::move(e));
    }

private:
    void doSendReqTo(Actor &receiver, EventHolder<Request> &&req, ActionCallback &&cb);

    void handleRequest(EventHolder<Request> &&req) {
        onRequest(std::move(req));
    }

    void handleRequestCallbackEvent(detail::RequestCallbackEvent &actionResult);

    void handleMessage(EventHolder<Message> &&message) {
        onMessage(std::move(message));
    }

    void handleNotice(EventHolder<Notice> &&notice) {
        onNotice(std::move(notice));
    }

    void handleTask(EventHolder<Task> &&task) {
        onTask(std::move(task));
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

    void doNotify(std::type_index type, EventHolder<Notice> &&notice);

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

    virtual void post(EventHolder<detail::Event> &&e) override {
        eventQueue_.push(std::move(e));
    }

    // 停止消息处理，释放消息处理线程，并等待线程释放完成
    // 子类必须在析构函数中显式调用，以确保在子类的组件释放前停止消息处理
    void stopAndJoin();

protected:
    std::atomic_bool stopped_{ false };

private:
    void loop();

    void updateThreadName();

    void stop();

private:
    BlockQueue<EventHolder<detail::Event>> eventQueue_;
    std::jthread recvThread_;
    std::atomic_bool nameModified_{ false };
};

namespace detail {

class SenderAwaredEvent : public Event {
public:
    SenderAwaredEvent() {}

    SenderAwaredEvent(const SenderAwaredEvent &other) {
        *this = other;
    }

    virtual ~SenderAwaredEvent() {}

    SenderAwaredEvent &operator=(const SenderAwaredEvent &other) {
        Event::operator=(other);
        sender_ = other.sender_;
        return *this;
    }

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

    Request(const Request &other) {
        *this = other;
    }

    Request &operator=(const Request &other) {
        detail::SenderAwaredEvent::operator=(other);
        callback_ = other.callback_;
    }

    virtual ~Request() {}

protected:
    virtual void handle(Actor &receiver, EventHolder<detail::Event> &&holder) override {
        receiver.handleRequest(std::move(holder));
    }

private:
    Callback callback_;

    friend class Actor;
};

using RequestHolder = EventHolder<Request>;


namespace detail {

class RunInActorEvent : public detail::Event {
public:
    RunInActorEvent(std::function<void()> &&runBody)
        : runBody_(std::move(runBody)) {}

    virtual ~RunInActorEvent() {}

protected:
    virtual void handle(Actor &receiver, EventHolder<detail::Event> &&holder) override {
        receiver.handleRunInActor(*this);
    }

private:
    std::function<void()> runBody_;

    friend class Actor;
};

class RequestCallbackEvent : public detail::Event {
public:
    using Callback = Request::Callback;

    RequestCallbackEvent(Callback &&callback, EventHolder<Response> &&resp)
        : callback_(std::move(callback)), resp_(std::move(resp)) {}

    virtual ~RequestCallbackEvent() {}

protected:
    virtual void handle(Actor &receiver, EventHolder<detail::Event> &&holder) override {
        receiver.handleRequestCallbackEvent(*this);
    }

private:
    Callback callback_;
    EventHolder<Response> resp_;

    friend class Actor;
};

} // namespace detail


class Message : public detail::SenderAwaredEvent {
public:
    Message() {}

    virtual ~Message() {}

protected:
    virtual void handle(Actor &receiver, EventHolder<detail::Event> &&holder) override {
        receiver.handleMessage(std::move(holder));
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
    virtual void handle(Actor &receiver, EventHolder<detail::Event> &&holder) override {
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
    virtual void handle(Actor &receiver, EventHolder<detail::Event> &&holder) override {
        receiver.handleNotice(std::move(holder));
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
    virtual void handle(Actor &receiver, EventHolder<detail::Event> &&holder) override {
        receiver.handleTask(std::move(holder));
    }

private:
    std::shared_ptr<ControllBlock> controllBlock_;
};

}
