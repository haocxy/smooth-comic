#pragma once

#include "core/declare_ptr.h"


namespace myapp {

class AsyncDeleter;

class PathManager;


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

private:
    DeclarePtr<AsyncDeleter> asyncDeleter_;
    DeclarePtr<PathManager> pathManager_;
};

}

