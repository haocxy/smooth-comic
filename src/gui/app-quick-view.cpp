#include "app-quick-view.h"

#include <QCoreApplication>
#include <QKeyEvent>
#include <QQmlEngine>
#include <QQmlContext>

#include "app-qml-engine.h"


namespace myapp {

AppQuickView::AppQuickView(AppQmlEngine &appQmlEngine, QWindow *parent)
    : QQuickView(&appQmlEngine, parent)
    , appQmlEngine_(appQmlEngine)
{
    appQmlEngine_.initView(*this);
}

AppQuickView::~AppQuickView()
{
}

void AppQuickView::keyReleaseEvent(QKeyEvent *e)
{
    if (appQmlEngine_.handleKeyRelease(e)) {
        return;
    } else {
        QQuickView::keyReleaseEvent(e);
    }
}

}
