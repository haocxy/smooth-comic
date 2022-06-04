#include "async-deleter.h"


namespace myapp {

AsyncDeleter::AsyncDeleter()
{
    setActorName("AsyncDeleter");
}

AsyncDeleter::~AsyncDeleter()
{
}

void AsyncDeleter::onMessage(actor::Message &msg)
{
    if (AsyncDeleteMsg *m = msg) {
        m->doDelete();
    }
}


}
