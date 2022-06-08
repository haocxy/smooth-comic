#include "qobject-actor.h"



namespace myapp {

QObjectActor::QObjectActor(QObject *parent)
    : QObject(parent)
{
    connect(this, &QObjectActor::qtSigRecvActorEvent, this, &QObjectActor::qtSlotRecvActorEvent, Qt::ConnectionType::QueuedConnection);
}

QObjectActor::~QObjectActor()
{
    
}

void QObjectActor::post(std::unique_ptr<actor::detail::Event> &&e)
{
    emit qtSigRecvActorEvent(MyApp_ActorEventWrapperForQtMetaObjSys(std::move(e)));
}

void QObjectActor::qtSlotRecvActorEvent(MyApp_ActorEventWrapperForQtMetaObjSys e)
{
    std::unique_ptr<actor::detail::Event> &eventPtr = e.ptr();
    if (eventPtr) {
        handleEvent(*eventPtr);
    }
}



QWidgetActor::QWidgetActor(QWidget *parent)
    : QWidget(parent)
{
    connect(this, &QWidgetActor::qtSigRecvActorEvent, this, &QWidgetActor::qtSlotRecvActorEvent, Qt::ConnectionType::QueuedConnection);
}

QWidgetActor::~QWidgetActor()
{
}

void QWidgetActor::post(std::unique_ptr<actor::detail::Event> &&e)
{
    emit qtSigRecvActorEvent(MyApp_ActorEventWrapperForQtMetaObjSys(std::move(e)));
}

void QWidgetActor::qtSlotRecvActorEvent(MyApp_ActorEventWrapperForQtMetaObjSys e)
{
    std::unique_ptr<actor::detail::Event> &eventPtr = e.ptr();
    if (eventPtr) {
        handleEvent(*eventPtr);
    }
}

}
