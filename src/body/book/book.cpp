#include "book.h"

#include <cstring>

#include "core/fs.h"
#include "core/logger.h"

#include "engine/async-deleter.h"

#include "book-cache.h"


namespace myapp {

using logger::gLogger;

Book::Book(Engine &engine, QObject *parent)
    : QObjectActor(parent)
    , engine_(engine) {

}

Book::~Book()
{
    asyncDeleteBookCache();
}

void Book::open(const fs::path &archiveFile)
{
    if (!fs::is_regular_file(archiveFile)) {
        return;
    }

    archiveFile_ = archiveFile;

    asyncDeleteBookCache();
    
    cache_ = new BookCache(engine_, archiveFile_);
    listen<BookCache::PageOpenedNotice>(*cache_);

    sendTo(*cache_, std::make_unique<BookCache::OpenBookMsg>());

    emit sigBookOpenStarted(QString::fromStdU32String(archiveFile_.generic_u32string()));
}

void Book::onNotice(actor::Notice &notice)
{
    if (BookCache::PageOpenedNotice *n = notice) {
        return handlePageOpenedNotice(*n);
    }
}

static std::u8string toU8String(const QString &qs)
{
    const std::string s = qs.toUtf8().toStdString();
    std::u8string u8s;
    u8s.resize(s.size());
    std::memcpy(u8s.data(), s.data(), s.size());
    return u8s;
}

void Book::asyncDeleteBookCache()
{
    if (cache_) {
        sendTo(engine_.asyncDeleter(), new AsyncDeleter::AsyncDeleteMsg(cache_.take()));
    }
}

void Book::handlePageOpenedNotice(const BookCache::PageOpenedNotice &n)
{
    for (const PageInfo &page : n.pages) {
        emit sigPageLoaded(QString::fromStdU32String(page.entryPath), page.width, page.height);
    }
}

}
