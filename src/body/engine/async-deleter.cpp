#include "async-deleter.h"


namespace myapp {

AsyncDeleter::AsyncDeleter(QObject *parent)
    : QObject(parent) {

    moveToThread(&mThread);

    connect(this, &AsyncDeleter::asyncDelete, this, [](QObject *obj) {
        delete obj;
    });

    mThread.setObjectName("AsyncDeleter");
    mThread.start();
}

AsyncDeleter::~AsyncDeleter()
{
    mThread.quit();
    mThread.wait();
}

}
