#pragma once

#include "util/actor.h"
#include "util/sqlite.h"


namespace myapp {

// 缩略图缓存
class ThumbCache : public actor::ThreadedActor {
public:
    ThumbCache(const fs::path &dbFile);

protected:
    void onActorStarted() override;

private:
    const fs::path dbFile_;
    sqlite::Database db_;
};


}

