#pragma once

#include "core/fs.h"
#include "util/actor.h"


namespace myapp {

class BookCache : public actor::ThreadedActor {
public:
    BookCache(fs::path cacheDir);

private:
    fs::path cacheDir_;
};

}
