#include "book.h"

#include <cstring>

#include "core/fs.h"
#include "core/logger.h"

#include "engine/path-manager.h"
#include "engine/async-deleter.h"


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

    cache_ = std::make_unique<BookCache>(archiveFile, engine_.pathManager().mkBookCacheDbFilePath(archiveFile));

    emit sigBookOpenStarted(QString::fromStdU32String(archiveFile.generic_u32string()));
}

void Book::onNotice(actor::EventHolder<actor::Notice> &&notice)
{
}

void Book::onRequest(actor::EventHolder<actor::Request> &&req)
{
    if (actor::EventHolder<GetThumbImgReq> r = std::move(req)) {
        return handleGetThumbImgReq(*r);
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

}

void Book::handleGetThumbImgReq(GetThumbImgReq &req)
{

}

}
