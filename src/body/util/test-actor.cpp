#include "test-actor.h"

#include "core/logger.h"


using namespace logger::global_loggers;

namespace myapp {

TestActor::TestActor()
{
    setActorName("TestActor");
}

TestActor::~TestActor()
{
}

std::unique_ptr<actor::Response> TestActor::onRequest(actor::Request &req)
{
    if (SumRequest *r = req.tryAs<SumRequest>()) {
        logInfo << "handle SumRequest(" << r->numA << ", " << r->numB << ")";
        return std::make_unique<SumRequest::Response>(r->numA + r->numB);
    }

    return nullptr;
}

}
