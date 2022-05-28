#pragma once

#include <QObject>

#include "engine/engine.h"
#include "book-loader.h"


namespace myapp {


class Book : public QObject {
    Q_OBJECT
public:
    explicit Book(Engine &engine, QObject *parent = nullptr);

    virtual ~Book();

private:
    Engine &mEngine;
    BookLoader mLoader;
};


}
