#include "book.h"

#include <cstring>

#include "util/fs.h"


namespace myapp {

Book::Book(Engine &engine, QObject *parent)
    : QObject(parent)
    , engine_(engine)
    , loader_(std::make_unique<BookLoader>(engine)) {

    initSignalsAndSlots();



    loader_->startLoadFromLocalFile("D:/tmp/a.zip");
}

Book::~Book()
{
    loader_->stop();

    disconnect();

    engine_.asyncDeleter().asyncDelete(std::move(loader_));
}

void Book::initSignalsAndSlots()
{
    connect(loader_.get(), &BookLoader::onPageLoaded, this, &Book::handleOnPageLoaded);
}

static std::u8string toU8String(const QString &qs)
{
    const std::string s = qs.toUtf8().toStdString();
    std::u8string u8s;
    u8s.resize(s.size());
    std::memcpy(u8s.data(), s.data(), s.size());
    return u8s;
}

void Book::handleOnPageLoaded(PageNum pageNum, const QImage &img)
{
    pageKeyToImg_[pageNum] = img;

    const fs::path outfile = QString("D:/tmp/save-by-class-Book/%1.bmp").arg(pageNum).toStdU32String();

    fs::create_directories(outfile.parent_path());

    img.save(QString::fromStdU32String(outfile.generic_u32string()), "BMP");
}

}
