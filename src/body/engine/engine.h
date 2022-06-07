#pragma once

#include "core/declare_ptr.h"

namespace myapp {

class AsyncDeleter;
class FsManager;
class ThumbCache;


class Engine {
public:
    Engine();

    ~Engine() {}

    AsyncDeleter &asyncDeleter() {
        return *asyncDeleter_;
    }

    FsManager &fsManager() {
        return *fsManager_;
    }

    ThumbCache &thumbCache() {
        return *thumbCache_;
    }

private:
    DeclarePtr<AsyncDeleter> asyncDeleter_;
    DeclarePtr<FsManager> fsManager_;
    DeclarePtr<ThumbCache> thumbCache_;
};

}

