#pragma once

#include <atomic>

#include <QObject>
#include <QPointer>

#include "util/autojoin-qthread.h"
#include "page-num.h"


namespace myapp {

class Engine;

class BookLoader : public QObject {
    Q_OBJECT
public:
    explicit BookLoader(Engine &engine, QObject *parent = nullptr);

    virtual ~BookLoader();

    void stop() {
        stopped_ = true;
    }

signals:
    void startLoadFromLocalFile(const QString &path);

    void onPageLoaded(const QString &pagePath, const QImage &img);

private:
    void initSignalsAndSlots();

    void doStartLoadFromLocalFile(const QString &filePath);

private:
    Engine &mEngine;
    AutojoinQThread mLoadThread;
    std::atomic_bool stopped_{ false };
};

}
