#include "actor.h"


namespace myapp::actor {
// 这部分是为业务逻辑提供的

void Actor::sendTo(Actor &receiver, std::unique_ptr<Action> action, std::function<void()> &&cb) {
    action->sender_ = this->handle_;
    action->callback_ = std::move(cb);
    receiver.post(std::unique_ptr<Event>(action.release()));
}

void Actor::handleAction(Action &action)
{
    dispatch(action);
    if (std::shared_ptr<Actor::Handle> senderHandle = action.sender_.lock()) {
        senderHandle->actor().post(std::make_unique<ActionResult>(std::move(action.callback_)));
    }
}

void Actor::handleActionResult(ActionResult &actionResult)
{
    if (actionResult.callback_) {
        actionResult.callback_();
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
    while (!stopped_) {
        std::optional<std::unique_ptr<Event>> evOpt = eventQueue_.pop();
        if (evOpt) {
            std::unique_ptr<Event> &ev = *evOpt;
            if (ev) {
                handleEvent(*ev);
            }
        }
    }
}

}
