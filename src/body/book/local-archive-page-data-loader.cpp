#include "local-archive-page-data-loader.h"

#include <QDebug>

#include "util/libarchive-wrapper.h"


namespace myapp {

LocalArchivePageDataLoader::LocalArchivePageDataLoader(const fs::path &archiveFile, const std::set<u32str> &loadedEntries)
    : archiveFile_(archiveFile)
    , loadedEntries_(loadedEntries)
{
}

LocalArchivePageDataLoader::~LocalArchivePageDataLoader()
{
    stopped_ = true;
    thread_.join();
}

void LocalArchivePageDataLoader::start()
{
    thread_ = jthread([this] { threadBody(); });
}

void LocalArchivePageDataLoader::threadBody()
{
    ThreadUtil::setNameForCurrentThread("LocalArchivePageDataLoader.LoadThread");

    wrapper::libarchive::Archive archive;

    try {
        archive.open(archiveFile_);
    } catch (const std::exception &) {
        sigFailed(BookError::BadArchive);
        return;
    }

    i32 pageCount = 0;

    PageNum seqNum = 0;

    while (!stopped_ && archive.nextEntry()) {
        qDebug() << "qmllog: archive.nextEntry, seqNum: " << seqNum << ", pageCount: " << pageCount;
        if (!archive.isRegularFile()) {
            continue;
        }
        ++pageCount;
        sptr<PageData> pageData = std::make_shared<PageData>();
        pageData->seqNum = seqNum++;
        pageData->name = archive.path();
        qDebug() << "qmllog: archive item: " << QString::fromStdU32String(pageData->name);
        if (loadedEntries_.contains(pageData->name)) {
            archive.skipContent();
        } else {
            pageData->data = archive.readContent();
            sigPageDataLoaded(pageData);
        }
    }

    sigPageCountDetected(pageCount);
}

}
