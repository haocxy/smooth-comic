#include "book-cache.h"

#include "core/logger.h"


namespace myapp {

using logger::gLogger;

BookCache::BookCache(const fs::path &archiveFile)
    : SingleThreadStrand("BookCache")
    , archiveFile_(archiveFile)
{

}

void BookCache::load()
{
    loader_ = std::make_unique<BookLoader>(archiveFile_);

    loaderSigConns_.clear();

    loaderSigConns_ += loader_->sigPageLoaded.connect([this](sptr<LoadedPage> page) {
        gLogger.d << "loader say: sigPageLoaded, " << static_cast<std::string>(u8str(page->name));
    });

    loaderSigConns_ += loader_->sigBookLoaded.connect([this]() {
        gLogger.d << "loader say: sigBookLoaded";
    });

    loader_->start();
}

}
