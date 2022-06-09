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
    stopAndJoin();
}

void TestActor::onRequest(actor::Request &req)
{
    ++handleRequestTimes_;

    notify(std::make_unique<RequestTimesNotice>(handleRequestTimes_));

    if (SumRequest *r = req) {
        gLogger.i << "handle SumRequest(" << r->numA << ", " << r->numB << ")";
        respondTo(std::move(req), new SumRequest::Response(r->numA + r->numB));
        return;
    }
}

}
