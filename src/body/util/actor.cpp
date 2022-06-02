#include "actor.h"


namespace myapp::actor {
// 这部分是为业务逻辑提供的

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

ThreadedActor::ThreadedActor()
    : recvThread_([this] { loop(); })
{

}

ThreadedActor::~ThreadedActor()
{
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
