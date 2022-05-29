#include "book.h"


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

void Book::handleOnPageLoaded(PageNum pageNum, const QImage &img)
{
    pageNumToImg_[pageNum] = img;

    QString file = QString::asprintf("D:/tmp/save/page-%2d.bmp", int(pageNum));



    img.save(file, "BMP");
}

}
