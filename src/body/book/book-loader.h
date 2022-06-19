#pragma once

#include <atomic>
#include <memory_resource>

#include "core/basetype.h"
#include "core/thread.h"

#include "util/sigconns.h"
#include "util/signal.h"
#include "util/memory.h"

#include "book-load-pipeline.h"
#include "page-sequencer.h"


namespace myapp {

class BookLoader : public SingleThreadStrand {
public:
    BookLoader(const fs::path &archiveFile);

    ~BookLoader();

    void start();

    using CbPageLoaded = void(sptr<LoadedPage> page);

    Signal<CbPageLoaded> sigPageLoaded;

    using CbBookLoaded = void();

    Signal<CbBookLoaded> sigBookLoaded;

private:
    void handlePageLoaded(sptr<LoadedPage> page);

    void handlePageCountDetected(i32 pageCount);

private:
    const fs::path archiveFile_;
    LimitedMemPool mempool_;
    uptr<BookLoadPipeline> bookLoadPipeline_;
    PageSequencer sequencer_;
    SigConns sigConns_;

    i32 loadedPageCount_ = 0;
    opt<i32> pageCount_;
    bool bookLoadedNotified_{ false };
};

}
