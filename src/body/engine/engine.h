#pragma once

#include "util/test-actor.h"
#include "async-deleter.h"


namespace myapp {

class Engine {
public:
    Engine();

    TestActor &testActor() {
        return testActor_;
    }

    AsyncDeleter &asyncDeleter() {
        return asyncDeleter_;
    }

private:
    TestActor testActor_;
    AsyncDeleter asyncDeleter_;
};

}

