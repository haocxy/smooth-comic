#pragma once

#include "actor.h"

#include <iostream>

#include "thread.h"


namespace myapp {

class SumAction : public actor::Request {
public:
    SumAction(int a, int b) : a(a), b(b) {}

    class Result : public actor::Result {
    public:
        Result(int n) : n(n) {}
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
    }

    virtual std::unique_ptr<actor::Result> dispatch(const actor::Request &action) override {
        if (const SumAction *a = action.tryAs<SumAction>()) {
            return std::unique_ptr<actor::Result>(new SumAction::Result(a->a + a->b));
        }

        return nullptr;
    }
};


class AskActor : public actor::ThreadedActor {
public:
    AskActor(actor::Actor &calcActor) : calcActor_(calcActor) {}

protected:

    virtual std::unique_ptr<actor::Result> dispatch(const actor::Request &action) override {
        return nullptr;
    }

    virtual void onActorStarted() override {
        ThreadUtil::setNameForCurrentThread("AskActor");

        sendTo<SumAction::Result>(calcActor_, std::unique_ptr<actor::Request>(new SumAction(1, 2)), [](SumAction::Result &r) {
            int sum = r.n;
            std::cout << "sum: " << sum << std::endl;
        });
    }

private:
    actor::Actor &calcActor_;
};




}
