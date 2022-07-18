#pragma once

#include "core/declare_ptr.h"


namespace myapp {

class AsyncDeleter;

class PathManager;

class CacheCleaner;


class Engine {
public:
    Engine();

    ~Engine() {}

    AsyncDeleter &asyncDeleter() {
        return *asyncDeleter_;
    }

    PathManager &pathManager() {
        return *pathManager_;
    }

    CacheCleaner &cacheCleaner() {
        return *cacheCleaner_;
    }

private:
    DeclarePtr<AsyncDeleter> asyncDeleter_;
    DeclarePtr<PathManager> pathManager_;
    DeclarePtr<CacheCleaner> cacheCleaner_;
};

}

