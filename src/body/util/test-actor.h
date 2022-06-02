#pragma once

#include "actor.h"


namespace myapp {

class TestActor : public actor::ThreadedActor {
public:

    class SumRequest : public actor::Request {
    public:

        SumRequest(int numA, int numB) : numA(numA), numB(numB) {}

        class Response : public actor::Response {
        public:
            Response(int result) : result(result) {}

            int result = 0;
        };

        int numA = 0;
        int numB = 0;
    };

    TestActor();

    virtual ~TestActor();

protected:
    virtual std::unique_ptr<actor::Response> onRequest(actor::Request &req) override;
};

}
