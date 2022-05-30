#include "actor.h"


namespace myapp::actor {
// 这部分是为业务逻辑提供的

void Actor::sendTo(Actor &receiver, std::unique_ptr<Request> action, ActionCallback &&cb) {
    action->sender_ = this->handle_;
    action->callback_ = std::move(cb);
    receiver.post(std::unique_ptr<Event>(action.release()));
}

void Actor::handleAction(Request &action)
{
    std::unique_ptr<Result> result = dispatch(action);

    if (std::shared_ptr<Actor::Handle> senderHandle = action.sender_.lock()) {

        std::unique_ptr<ActionResult> resultEvent = std::make_unique<ActionResult>(
            std::move(action.callback_), std::move(result));

        senderHandle->actor().post(std::move(resultEvent));
    }
}

void Actor::handleActionResult(ActionResult &actionResult)
{
    if (actionResult.callback_ && actionResult.result_) {
        actionResult.callback_(*actionResult.result_);
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
        std::optional<std::unique_ptr<Event>> evOpt = eventQueue_.pop();
        if (evOpt) {
            std::unique_ptr<Event> &ev = *evOpt;
            if (ev) {
                handleEvent(*ev);
            }
        }
    }

    onActorStopped();
}

}
