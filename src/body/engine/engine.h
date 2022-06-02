#pragma once

#include "async-deleter.h"


namespace myapp {

class Engine {
public:
    Engine();

    AsyncDeleter &asyncDeleter() {
        return mAsyncDeleter;
    }

private:
    AsyncDeleter mAsyncDeleter;
};

}

