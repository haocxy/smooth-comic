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

void TestActor::onRequest(actor::EventHolder<actor::Request> &&req)
{
    ++handleRequestTimes_;

    notify(new RequestTimesNotice(handleRequestTimes_));

    if (actor::EventHolder<SumRequest> r = std::move(req)) {
        gLogger.i << "handle SumRequest(" << r->numA << ", " << r->numB << ")";
        int result = r->numA + r->numB;
        respondTo(std::move(r), new SumRequest::Response(result));
        return;
    }
}

}
