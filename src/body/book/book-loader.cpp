#include "book-loader.h"

#include "engine/engine.h"

#include "img-archive.h"


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
    ImgArchive archive;
    try {
        archive.load(fs::path(path.toStdWString()), [](int32_t i, const QString &path, const QImage &img) {
            int n = 0;
            });
    }
    catch (const std::exception &e) {
        std::string s = e.what();
        int n = 0;
    }

    //QPdfDocument doc;
    //doc.load(path);

    //const int pageCount = doc.pageCount();

    //for (int pageIndex = 0; !stopped_ && pageIndex < pageCount; ++pageIndex) {
    //    QPdfSelection pageSelection = doc.getAllText(pageIndex);
    //    QRectF boundingRect = pageSelection.boundingRectangle();

    //    QImage img = doc.render(pageIndex, { 800, 600 });
    //    if (!stopped_) {
    //        emit onPageLoaded(pageIndex, img);
    //    }
    //}
}

}
