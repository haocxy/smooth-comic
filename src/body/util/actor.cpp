#include "actor.h"


namespace myapp::actor {
// 这部分是为业务逻辑提供的

void Actor::sendTo(Actor &receiver, std::unique_ptr<Request> req, ActionCallback &&cb) {
    req->sender_ = this->handle_;
    req->callback_ = std::move(cb);
    receiver.post(std::move(req));
}

void Actor::handleRequest(Request &action)
{
    std::unique_ptr<Response> result = dispatch(action);

    if (std::shared_ptr<Actor::Handle> senderHandle = action.sender_.lock()) {

        std::unique_ptr<RequestCallbackEvent> resultEvent = std::make_unique<RequestCallbackEvent>(
            std::move(action.callback_), std::move(result));

        senderHandle->actor().post(std::move(resultEvent));
    }
}

void Actor::handleRequestCallbackEvent(RequestCallbackEvent &actionResult)
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

}
