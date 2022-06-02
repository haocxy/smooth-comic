#pragma once

#include <map>

#include <QObject>
#include <QHash>
#include <QImage>

#include "engine/engine.h"
#include "util/qobject-actor.h"

#include "book-loader.h"
#include "page-num.h"


namespace myapp {


class Book : public QObjectActor {
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
    std::map<PageNum, QImage> pageKeyToImg_;
};


}
