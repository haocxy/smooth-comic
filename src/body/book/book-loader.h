#pragma once

#include <atomic>
#include <memory_resource>

#include "core/basetype.h"
#include "core/thread.h"

#include "util/sigconns.h"
#include "util/signal.hpp"
#include "util/memory.h"

#include "book-load-pipeline.h"


namespace myapp {

class BookLoader : public SingleThreadStrand {
public:
    BookLoader(const fs::path &archiveFile, const std::set<u32str> &loadedEntries);

    ~BookLoader();

    void start();

    using CbPageLoaded = void(sptr<LoadedPage> page);

    Signal<CbPageLoaded> sigPageLoaded;

    using CbBookLoaded = void(i32 totalPageCount);

    Signal<CbBookLoaded> sigBookLoaded;

    using CbFailed = void(BookError err);

    Signal<CbFailed> sigFailed;

private:
    void handlePageLoaded(sptr<LoadedPage> page);

    void handlePageCountDetected(i32 pageCount);

private:
    const fs::path archiveFile_;

    uptr<BookLoadPipeline> bookLoadPipeline_;
    SigConns sigConns_;

    i32 loadedPageCount_ = 0;
    opt<i32> pageCount_;
    bool bookLoadedNotified_{ false };
};

}
