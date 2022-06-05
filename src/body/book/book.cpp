#include "book.h"

#include <cstring>

#include "core/fs.h"
#include "core/logger.h"

#include "engine/async-deleter.h"


namespace myapp {

Book::Book(Engine &engine, QObject *parent)
    : QObjectActor(parent)
    , engine_(engine)
    , loader_(std::make_unique<BookLoader>()) {

    listen<BookLoader::PageLoaded>(*loader_);

    sendTo(*loader_, std::make_unique<BookLoader::StartLoadMsg>("D:/tmp/a.zip"));
}

Book::~Book()
{
    disconnect();

    engine_.asyncDeleter().post(std::make_unique<AsyncDeleter::AsyncDeleteMsg>(std::move(loader_)));
}

void Book::onNotice(actor::Notice &notice)
{
    if (BookLoader::PageLoaded *n = notice) {
        handleOnPageLoaded(n->pageNum, n->img);
        return;
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

void Book::handleOnPageLoaded(PageNum pageNum, const QPixmap &img)
{
    QPixmap &page = pageKeyToImg_[pageNum];

    page = img;

    emit sigPageLoaded(pageNum, page, static_cast<PageNum>(pageKeyToImg_.size()));
}

}
