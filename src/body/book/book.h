#pragma once

#include <map>

#include <QObject>
#include <QHash>
#include <QPixmap>

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

signals:
    void sigPageLoaded(PageNum pageNum, const QPixmap &img, PageNum totalPages);

protected:
    virtual void onNotice(actor::Notice &notice) override;

private:
    void handleOnPageLoaded(PageNum pageNum, const QPixmap &img);

private:
    Engine &engine_;
    std::unique_ptr<BookLoader> loader_;
    std::map<PageNum, QPixmap> pageKeyToImg_;
};


}
