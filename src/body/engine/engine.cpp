#include "engine.h"

#include "book/thumb-cache.h"

#include "async-deleter.h"
#include "fs-manager.h"


namespace myapp {

Engine::Engine() {
    const fs::path cacheDir = "D:/smooth-comic-cache";
    asyncDeleter_.emplace();
    fsManager_.emplace();
    thumbCache_.emplace(cacheDir / "thumb-cache.db");
}

}
