#pragma once

#include <QObject>
#include <QPointer>

#include "util/autojoin-qthread.h"


namespace myapp {

class Engine;

class BookLoader : public QObject {
    Q_OBJECT
public:
    explicit BookLoader(Engine &engine, QObject *parent = nullptr);

    virtual ~BookLoader();

private:
    Engine &mEngine;
    QPointer<AutojoinQThread> mLoadThread;
};

}
