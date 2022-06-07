#include "book.h"

#include <cstring>

#include "core/fs.h"
#include "core/logger.h"

#include "engine/async-deleter.h"


namespace myapp {

using logger::gLogger;

Book::Book(Engine &engine, QObject *parent)
    : QObjectActor(parent)
    , engine_(engine) {}

Book::~Book()
{
}

void Book::open(const fs::path &archiveFile)
{
    gLogger.e << "Book::open() unimplemented";
}

static std::u8string toU8String(const QString &qs)
{
    const std::string s = qs.toUtf8().toStdString();
    std::u8string u8s;
    u8s.resize(s.size());
    std::memcpy(u8s.data(), s.data(), s.size());
    return u8s;
}

void Book::handleOnPageLoaded(PageNum pageNum, const QPixmap &img)
{
    QPixmap &page = pageKeyToImg_[pageNum];

    page = img;

    emit sigPageLoaded(pageNum, page, static_cast<PageNum>(pageKeyToImg_.size()));
}

}
