#include "book-loader.h"

#include "engine/engine.h"

#include "img-archive.h"


namespace myapp {



BookLoader::BookLoader(Engine &engine, QObject *parent)
    : QObject(parent)
    , mEngine(engine)
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

void BookLoader::doStartLoadFromLocalFile(const QString &filePath)
{
    ImgArchive archive;
    archive.load(filePath.toStdU32String(), [this](const QString &pagePath, const QImage &img) {
        emit onPageLoaded(pagePath, img);
        return !stopped_;
    });
}

}
