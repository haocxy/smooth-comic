#include "book-loader.h"

#include "local-archive-page-data-loader.h"


namespace myapp {

BookLoader::BookLoader(const fs::path &archiveFile)
    : SingleThreadStrand("BookLoader")
    , archiveFile_(archiveFile)
    , mempool_(1024 * 1024 * 100)
{
    uptr<LocalArchivePageDataLoader> pageDataLoader
        = std::make_unique<LocalArchivePageDataLoader>(archiveFile_, &mempool_);

    bookLoadPipeline_ = std::make_unique<BookLoadPipeline>(
        std::move(pageDataLoader), &mempool_);

    sigConns_ += bookLoadPipeline_->sigPageLoaded.connect([this](sptr<LoadedPage> page) {
        exec([this, page] {
            ++loadedPageCount_;
            sigPageLoaded(page);
            if (!bookLoadedNotified_ && pageCount_.has_value() && loadedPageCount_ >= *pageCount_) {
                sigBookLoaded();
                bookLoadedNotified_ = true;
            }
        });
    });

    sigConns_ += bookLoadPipeline_->sigPageCountDetected.connect([this](i32 pageCount) {
        exec([this, pageCount] {
            pageCount_ = pageCount;
            if (!bookLoadedNotified_ && loadedPageCount_ >= *pageCount_) {
                sigBookLoaded();
                bookLoadedNotified_ = true;
            }
        });
    });
}

BookLoader::~BookLoader()
{
}

void BookLoader::start()
{
    bookLoadPipeline_->start();
}

}
