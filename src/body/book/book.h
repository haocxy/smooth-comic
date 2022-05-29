#pragma once

#include <QObject>
#include <QHash>
#include <QImage>

#include "engine/engine.h"
#include "page-num.h"
#include "book-loader.h"


namespace myapp {


class Book : public QObject {
    Q_OBJECT
public:

    explicit Book(Engine &engine, QObject *parent = nullptr);

    virtual ~Book();

private:
    void initSignalsAndSlots();

    void handleOnPageLoaded(PageNum pageNum, const QImage &img);

private:
    Engine &engine_;
    std::unique_ptr<BookLoader> loader_;
    QHash<PageNum, QImage> pageNumToImg_;
};


}
