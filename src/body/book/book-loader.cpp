#include "book-loader.h"

#include "engine/engine.h"


namespace myapp {



BookLoader::BookLoader(Engine &engine, QObject *parent)
    : mEngine(engine)
{
    mLoadThread = new AutojoinQThread;

    mLoadThread->setObjectName("BookLoader");

    moveToThread(mLoadThread);

    mLoadThread->start();
}

BookLoader::~BookLoader()
{
    mEngine.asyncDeleter().asyncDelete(mLoadThread);
}

}
