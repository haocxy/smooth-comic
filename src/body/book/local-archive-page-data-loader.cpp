#include "local-archive-page-data-loader.h"

#include "util/libarchive-wrapper.h"


namespace myapp {

LocalArchivePageDataLoader::LocalArchivePageDataLoader(const fs::path &archiveFile, Allocator allocator)
    : archiveFile_(archiveFile)
    , allocator_(allocator)
{
}

LocalArchivePageDataLoader::~LocalArchivePageDataLoader()
{
    stopped_ = true;
    thread_.join();
}

void LocalArchivePageDataLoader::start()
{
    thread_ = std::jthread([this] { threadBody(); });
}

void LocalArchivePageDataLoader::threadBody()
{
    ThreadUtil::setNameForCurrentThread("LocalArchivePageDataLoader.LoadThread");

    wrapper::libarchive::Archive archive(archiveFile_, allocator_);

    i32 pageCount = 0;

    PageNum seqNum = 1;

    while (!stopped_ && archive.nextEntry()) {
        ++pageCount;
        sptr<PageData> pageData = std::make_shared<PageData>();
        pageData->seqNum = seqNum++;
        pageData->name = archive.path();
        pageData->data = archive.readContent();
        sigPageDataLoaded(pageData);
    }

    sigPageCountDetected(pageCount);
}

}
