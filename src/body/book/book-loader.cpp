#include "book-loader.h"

#include <QPdfDocument>

#include "engine/engine.h"


namespace myapp {



BookLoader::BookLoader(Engine &engine, QObject *parent)
    : mEngine(engine)
{

    mLoadThread.setObjectName("BookLoader");

    moveToThread(&mLoadThread);

    initSignalsAndSlots();

    mLoadThread.start();
}

BookLoader::~BookLoader()
{
    disconnect();

    stopped_ = true;
}

void BookLoader::initSignalsAndSlots()
{
    using Class = BookLoader;

    connect(this, &Class::startLoadFromLocalFile, this, &Class::doStartLoadFromLocalFile);
}

void BookLoader::doStartLoadFromLocalFile(const QString &path)
{
    QPdfDocument doc;
    doc.load(path);

    const int pageCount = doc.pageCount();

    for (int pageIndex = 0; !stopped_ && pageIndex < pageCount; ++pageIndex) {
        QPdfSelection pageSelection = doc.getAllText(pageIndex);
        QRectF boundingRect = pageSelection.boundingRectangle();

        QImage img = doc.render(pageIndex, { 800, 600 });
        if (!stopped_) {
            emit onPageLoaded(pageIndex, img);
        }
    }
}

}
