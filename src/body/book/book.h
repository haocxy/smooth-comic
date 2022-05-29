#pragma once

#include <map>

#include <QObject>
#include <QHash>
#include <QImage>

#include "engine/engine.h"

#include "book-loader.h"
#include "page-num.h"
#include "page-key.h"


namespace myapp {


class Book : public QObject {
    Q_OBJECT
public:

    explicit Book(Engine &engine, QObject *parent = nullptr);

    virtual ~Book();

private:
    void initSignalsAndSlots();

    void handleOnPageLoaded(const QString &pagePath, const QImage &img);

private:
    Engine &engine_;
    std::unique_ptr<BookLoader> loader_;
    std::map<PageKey, QImage, PageKey::CmpAsc> pageKeyToImg_;
};


}
