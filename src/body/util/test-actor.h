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

    class RequestTimesNotice : public actor::Notice {
    public:
        RequestTimesNotice() {}

        RequestTimesNotice(int times) : times(times) {}

        RequestTimesNotice(const RequestTimesNotice &other)
            : actor::Notice(other), times(other.times) {}

        virtual RequestTimesNotice *clone() const override {
            return new RequestTimesNotice(*this);
        }

        int times = 0;
    };

    TestActor();

    virtual ~TestActor();

protected:
    void onRequest(actor::Request &req) override;

private:
    int handleRequestTimes_{};
};

}
