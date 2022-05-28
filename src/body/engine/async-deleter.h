#pragma once

#include <memory>

#include <QObject>
#include <QThread>


namespace myapp {

class AsyncDeleter : public QObject {
    Q_OBJECT
public:
    explicit AsyncDeleter(QObject *parent = nullptr);

    virtual ~AsyncDeleter();

signals:
    void asyncDelete(QObject *obj);

private:
    QThread mThread;
};


}

