#pragma once

#include "actor.h"

#include "core/logger.h"


using namespace logger::global_loggers;

namespace myapp::demo::actor {

namespace actor = myapp::actor;

class SumRequest : public actor::Request {
public:
    SumRequest(int a, int b) : a(a), b(b) {}

    class Response : public actor::Response {
    public:
        Response(int n) : n(n) {}
        int n{};
    };

    int a{};
    int b{};
};

class ActorStartedMessage : public actor::Message {
public:
    ActorStartedMessage(const std::string &actorName) : actorName(actorName) {}
    std::string actorName;
};

class CalcActor : public actor::ThreadedActor {
public:

protected:
    virtual void onActorStarted() override {
        ThreadUtil::setNameForCurrentThread("CalcActor");
        logInfo << "CalcActor started";
    }

    virtual std::unique_ptr<actor::Response> onRequest(const actor::Request &action) override {
        if (const SumRequest *a = action.tryAs<SumRequest>()) {
            logInfo << "CalcActor handle SumRequest(" << a->a << ", " << a->b << ")";
            return std::unique_ptr<actor::Response>(new SumRequest::Response(a->a + a->b));
        }

        return nullptr;
    }

    virtual void onMessage(const actor::Message &msg) override {
        if (const ActorStartedMessage *m = msg.tryAs<ActorStartedMessage>()) {
            logInfo << "CalcActor handle ActorStartedMessage(" << m->actorName << ")";
        }
    }
};


class AskActor : public actor::ThreadedActor {
public:
    AskActor(actor::Actor &calcActor) : calcActor_(calcActor) {}

protected:

    virtual void onActorStarted() override {
        ThreadUtil::setNameForCurrentThread("AskActor");

        logInfo << "AskActor started";

        sendTo(calcActor_, std::make_unique<ActorStartedMessage>("AskActor"));

        sendTo(calcActor_, std::make_unique<SumRequest>(1, 2), [](SumRequest::Response &r) {
            logInfo << "AskActor handle SumRequest::Response(" << r.n << ")";
        });
    }

private:
    actor::Actor &calcActor_;
};


void actor_demo_01();


}
