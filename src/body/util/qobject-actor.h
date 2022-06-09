#pragma once

#include <QObject>
#include <QWidget>

#include "actor.h"


class MyApp_ActorEventWrapperForQtMetaObjSys {
public:
    MyApp_ActorEventWrapperForQtMetaObjSys() {}

    MyApp_ActorEventWrapperForQtMetaObjSys(myapp::actor::EventHolder<myapp::actor::detail::Event> &&ptr)
        : ptr_(std::move(ptr)) {}

    MyApp_ActorEventWrapperForQtMetaObjSys(const MyApp_ActorEventWrapperForQtMetaObjSys &other)
        : ptr_(std::move(const_cast<MyApp_ActorEventWrapperForQtMetaObjSys &>(other).ptr_)) {}

    MyApp_ActorEventWrapperForQtMetaObjSys &operator=(const MyApp_ActorEventWrapperForQtMetaObjSys &other) {
        if (this != &other) {
            ptr_ = std::move(const_cast<MyApp_ActorEventWrapperForQtMetaObjSys &>(other).ptr_);
        }
        return *this;
    }

    myapp::actor::EventHolder<myapp::actor::detail::Event> &ptr() {
        return ptr_;
    }

private:
    myapp::actor::EventHolder<myapp::actor::detail::Event> ptr_;
};

Q_DECLARE_METATYPE(MyApp_ActorEventWrapperForQtMetaObjSys);


namespace myapp {

class QObjectActor : public QObject, public actor::Actor {
    Q_OBJECT

public:
    explicit QObjectActor(QObject *parent);

    virtual ~QObjectActor();

signals:
    void qtSigRecvActorEvent(MyApp_ActorEventWrapperForQtMetaObjSys e);

private slots:
    void qtSlotRecvActorEvent(MyApp_ActorEventWrapperForQtMetaObjSys e);

protected:
    virtual void post(actor::EventHolder<actor::detail::Event> &&e) override;
};

class QWidgetActor : public QWidget, public actor::Actor {
    Q_OBJECT

public:
    explicit QWidgetActor(QWidget *parent);

    virtual ~QWidgetActor();

signals:
    void qtSigRecvActorEvent(MyApp_ActorEventWrapperForQtMetaObjSys e);

private slots:
    void qtSlotRecvActorEvent(MyApp_ActorEventWrapperForQtMetaObjSys e);

protected:
    virtual void post(actor::EventHolder<actor::detail::Event> &&e) override;

};

}
