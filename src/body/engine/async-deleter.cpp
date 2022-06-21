#include "async-deleter.h"


namespace myapp {

AsyncDeleter::AsyncDeleter()
    : worker_("AsyncDeleter")
{
}

AsyncDeleter::~AsyncDeleter()
{
    worker_.stopEventQueue();
}

}
