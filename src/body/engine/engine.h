#pragma once

#include "async-deleter.h"


namespace myapp {

class Engine {
public:
    Engine();

    AsyncDeleter &asyncDeleter() {
        return asyncDeleter_;
    }

private:
    AsyncDeleter asyncDeleter_;
};

}

