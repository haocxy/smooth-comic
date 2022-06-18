#pragma once

#include <atomic>
#include <memory_resource>

#include "core/basetype.h"
#include "core/thread.h"

#include "util/sigconns.h"
#include "util/signal.h"

#include "book-load-pipeline.h"


namespace myapp {

class BookLoader : public SingleThreadStrand {
public:
    BookLoader(const fs::path &archiveFile);

    void start();

    using CbPageLoaded = void(sptr<LoadedPage> page);

    Signal<CbPageLoaded> sigPageLoaded;

    using CbBookLoaded = void();

    Signal<CbBookLoaded> sigBookLoaded;

private:
    const fs::path archiveFile_;
    std::pmr::synchronized_pool_resource mempool_;
    uptr<BookLoadPipeline> bookLoadPipeline_;
    SigConns sigConns_;

    i32 loadedPageCount_ = 0;
    opt<i32> pageCount_;
    bool bookLoadedNotified_{ false };
};

}
