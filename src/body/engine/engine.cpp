#include "engine.h"


#include "async-deleter.h"
#include "thumb-cache.h"


namespace {

template <typename T>
void del(T *&ptr)
{
    delete ptr;
    ptr = nullptr;
}

}


namespace myapp {

Engine::Engine() {
    asyncDeleter_ = new AsyncDeleter;
    thumbCache_ = new ThumbCache;
}

Engine::~Engine()
{
    del(thumbCache_);
    del(asyncDeleter_);
}



}
