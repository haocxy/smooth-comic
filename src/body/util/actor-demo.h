#pragma once

#include "actor.h"

#include "core/logger.h"


namespace myapp::demo::actor {

namespace actor = myapp::actor;

using logger::gLogger;

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
    CalcActor() {
        setActorName("CalcActor");
    }

    virtual ~CalcActor() {
        stopAndJoin();
    }

protected:
    virtual void onActorStarted() override {
        ThreadUtil::setNameForCurrentThread("CalcActor");
        gLogger.i << "CalcActor started";
    }

    virtual std::unique_ptr<actor::Response> onRequest(actor::Request &action) override {
        if (const SumRequest *a = action) {
            gLogger.i << "CalcActor handle SumRequest(" << a->a << ", " << a->b << ")";
            return std::unique_ptr<actor::Response>(new SumRequest::Response(a->a + a->b));
        }

        return nullptr;
    }

    virtual void onMessage(actor::Message &msg) override {
        if (const ActorStartedMessage *m = msg) {
            gLogger.i << "CalcActor handle ActorStartedMessage(" << m->actorName << ")";
        }
    }
};


class AskActor : public actor::ThreadedActor {
public:
    AskActor(actor::Actor &calcActor) : calcActor_(calcActor) {
        setActorName("AskActor");
    }

    virtual ~AskActor() {
        stopAndJoin();
    }



protected:

    virtual void onActorStarted() override {
        ThreadUtil::setNameForCurrentThread("AskActor");

        gLogger.i << "AskActor started";

        sendTo(calcActor_, std::make_unique<ActorStartedMessage>("AskActor"));

        sendTo(calcActor_, std::make_unique<SumRequest>(1, 2), [](SumRequest::Response &r) {
            gLogger.i << "AskActor handle SumRequest::Response(" << r.n << ")";
        });
    }

private:
    actor::Actor &calcActor_;
};


void actor_demo_01();


}
