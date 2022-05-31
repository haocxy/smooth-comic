#pragma once

#include "actor.h"

#include "core/logger.h"


using namespace logger::global_loggers;

namespace myapp {

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

class CalcActor : public actor::ThreadedActor {
public:

protected:
    virtual void onActorStarted() override {
        ThreadUtil::setNameForCurrentThread("CalcActor");
        logInfo << "CalcActor started";
    }

    virtual std::unique_ptr<actor::Response> dispatch(const actor::Request &action) override {
        if (const SumRequest *a = action.tryAs<SumRequest>()) {
            logInfo << "CalcActor handle SumRequest(" << a->a << ", " << a->b << ")";
            return std::unique_ptr<actor::Response>(new SumRequest::Response(a->a + a->b));
        }

        return nullptr;
    }
};


class AskActor : public actor::ThreadedActor {
public:
    AskActor(actor::Actor &calcActor) : calcActor_(calcActor) {}

protected:

    virtual void onActorStarted() override {
        ThreadUtil::setNameForCurrentThread("AskActor");

        logInfo << "AskActor started";

        sendTo(calcActor_, std::make_unique<SumRequest>(1, 2), [](SumRequest::Response &r) {
            logInfo << "AskActor handle SumRequest::Response(" << r.n << ")";
        });
    }

private:
    actor::Actor &calcActor_;
};




}
