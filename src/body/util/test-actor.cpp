#include "test-actor.h"

#include "core/logger.h"


namespace myapp {

using logger::gLogger;

TestActor::TestActor()
{
    setActorName("TestActor");
}

TestActor::~TestActor()
{
    stopEventHandle();
}

std::unique_ptr<actor::Response> TestActor::onRequest(actor::Request &req)
{
    ++handleRequestTimes_;

    notify(std::make_unique<RequestTimesNotice>(handleRequestTimes_));

    if (SumRequest *r = req) {
        gLogger.i << "handle SumRequest(" << r->numA << ", " << r->numB << ")";
        return std::make_unique<SumRequest::Response>(r->numA + r->numB);
    }



    return nullptr;
}

}
