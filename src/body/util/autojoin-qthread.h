#pragma once

#include <QThread>


namespace myapp {

class AutojoinQThread : public QThread {
    Q_OBJECT
public:
    explicit AutojoinQThread(QObject *parent = nullptr)
        : QThread(parent) {}

    virtual ~AutojoinQThread() {
        quit();
        wait();
    }
};

}
