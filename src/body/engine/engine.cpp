#include "engine.h"


#include "async-deleter.h"
#include "book/thumb-cache.h"


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
    const fs::path cacheDir = "D:/smooth-comic-cache";
    asyncDeleter_ = new AsyncDeleter;
    thumbCache_ = new ThumbCache(cacheDir / "thumb-cache.db");
}

Engine::~Engine()
{
    del(thumbCache_);
    del(asyncDeleter_);
}



}
