#pragma once

#include <functional>
#include <memory>

#include <QObject>
#include <QThread>

#include "core/thread.h"


class MyApp_TaskWrapperForQtPlatform {
public:
    MyApp_TaskWrapperForQtPlatform() {}

    explicit MyApp_TaskWrapperForQtPlatform(std::function<void()> &&task)
        : task(std::make_shared<std::function<void()>>(std::move(task))) {}

    void call() {
        if (task) {
            (*task)();
        }
    }

    std::shared_ptr<std::function<void()>> task;
};

Q_DECLARE_METATYPE(MyApp_TaskWrapperForQtPlatform);


namespace myapp {

class QtObjStrandEntry : public QObject, public StrandEntry {
    Q_OBJECT
public:
    QtObjStrandEntry(QObject *parent = nullptr) : QObject(parent) {
        connect(this, &QtObjStrandEntry::qtSigStrandEntryPost, this, &QtObjStrandEntry::qtSlotStrandEntryPost);
    }

    virtual ~QtObjStrandEntry() {}

    virtual void post(Task &&task) override {
        emit qtSigStrandEntryPost(MyApp_TaskWrapperForQtPlatform(std::move(task)));
    }

signals:
    void qtSigStrandEntryPost(MyApp_TaskWrapperForQtPlatform task);

protected:
    virtual bool inThread() const override {
        return false;
    }

private:
    void qtSlotStrandEntryPost(MyApp_TaskWrapperForQtPlatform task) {
        task.call();
    }
};

}
