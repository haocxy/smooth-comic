#include "engine.h"

#include "async-deleter.h"
#include "path-manager.h"


namespace myapp {

Engine::Engine() {
    asyncDeleter_ = new AsyncDeleter;
    pathManager_ = new PathManager;
}

}
