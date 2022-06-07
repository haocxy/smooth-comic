#include "actor.h"

#include "core/logger.h"


namespace myapp::actor {

using logger::gLogger;

void Actor::post(std::function<void()> &&action) {
    post(std::make_unique<detail::RunInActorEvent>(std::move(action)));
}

void Actor::doSendTo(Actor &receiver, std::unique_ptr<Request> req, ActionCallback &&cb) {
    req->sender_ = this->handle_;
    req->callback_ = std::move(cb);
    receiver.post(std::move(req));
}

void Actor::handleRequest(Request &req)
{   
    std::unique_ptr<Response> result = onRequest(req);

    if (result) {

        if (std::shared_ptr<Actor::Handle> senderHandle = req.sender_.lock()) {

            std::unique_ptr<detail::RequestCallbackEvent> resultEvent = std::make_unique<detail::RequestCallbackEvent>(
                std::move(req.callback_), std::move(result));

            senderHandle->actor().post(std::move(resultEvent));
        }

    }
}

void Actor::handleRequestCallbackEvent(detail::RequestCallbackEvent &actionResult)
{
    if (actionResult.callback_ && actionResult.resp_) {
        actionResult.callback_(*actionResult.resp_);
    }
}

void Actor::handleRunInActor(detail::RunInActorEvent &runInActorEvent) {
    if (runInActorEvent.runBody_) {
        runInActorEvent.runBody_();
    }
}

void Actor::handleAddListenerMessage(detail::AddListenerMessage &msg) {
    HandleSet &handles = listeners_[msg.noticeType()];
    std::shared_ptr<Handle> sender = msg.sender();
    if (!handles.contains(sender)) {
        handles.insert(sender);
    }
}

void Actor::doAddListenerTo(Actor &receiver, const std::type_index &noticeType)
{
    std::unique_ptr<detail::AddListenerMessage> msg = std::make_unique<detail::AddListenerMessage>(noticeType);
    sendTo(receiver, std::move(msg));
}

void Actor::doNotify(std::type_index type, std::unique_ptr<Notice> &&notice)
{
    notice->setSender(handle_);

    std::vector<std::weak_ptr<Handle>> badHandles;

    HandleSet &handles = listeners_[type];
    for (std::weak_ptr<Handle> weakHandle : handles) {
        std::shared_ptr<Handle> handle = weakHandle.lock();
        if (handle) {
            handle->actor().post(std::unique_ptr<Notice>(notice->clone()));
        } else {
            badHandles.push_back(weakHandle);
        }
    }

    for (std::weak_ptr<Handle> badHandle : badHandles) {
        handles.erase(badHandle);
    }
}

ThreadedActor::ThreadedActor()
    : recvThread_([this] { loop(); })
{

}

ThreadedActor::~ThreadedActor()
{
    gLogger.d << "destruct actor: " << actorName();

    stopped_ = true;
    eventQueue_.stop();
}

void ThreadedActor::loop()
{
    updateThreadName();

    onActorStarted();

    while (!stopped_) {
        std::optional<std::unique_ptr<detail::Event>> evOpt = eventQueue_.pop();
        if (evOpt) {
            std::unique_ptr<detail::Event> &ev = *evOpt;
            if (ev) {
                handleEvent(*ev);
            }
        }
    }

    onActorStopped();
}

void ThreadedActor::updateThreadName()
{
    if (!nameModified_) {
        return;
    }

    ThreadUtil::setNameForCurrentThread(actorName());
    nameModified_ = false;
}

}
