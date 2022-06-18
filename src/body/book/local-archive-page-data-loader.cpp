#include "local-archive-page-data-loader.h"

#include "util/libarchive-wrapper.h"


namespace myapp {

LocalArchivePageDataLoader::LocalArchivePageDataLoader(const fs::path &archiveFile, Allocator allocator)
    : archiveFile_(archiveFile)
    , allocator_(allocator)
{
}

void LocalArchivePageDataLoader::start()
{
    thread_ = std::jthread([this] { threadBody(); });
}

void LocalArchivePageDataLoader::threadBody()
{
    ThreadUtil::setNameForCurrentThread("LocalArchivePageDataLoader.LoadThread");

    wrapper::libarchive::Archive archive(archiveFile_, allocator_);

    while (archive.nextEntry()) {
        u32str entryName = archive.path();
        sptr<scc::buff> data = std::make_shared<scc::buff>(archive.readContent());
        sigPageDataLoaded(entryName, data);
    }

    sigAllPageLoaded();
}

}
