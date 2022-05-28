#pragma once

#include <QObject>

#include "async-deleter.h"


namespace myapp {

class Engine : public QObject {
    Q_OBJECT
public:
    explicit Engine(QObject *parent = nullptr);

    AsyncDeleter &asyncDeleter() {
        return mAsyncDeleter;
    }

private:
    AsyncDeleter mAsyncDeleter;
};

}

