#include "app-quick-view.h"

#include <QCoreApplication>
#include <QKeyEvent>
#include <QQmlEngine>
#include <QQmlContext>
#include <QQuickItem>

#include "app-qml-engine.h"


namespace myapp {

AppQuickView::AppQuickView(AppQmlEngine &appQmlEngine, QWindow *parent)
    : QQuickView(&appQmlEngine, parent)
    , appQmlEngine_(appQmlEngine)
{
    setMinimumSize(QSize(800, 600));

    appQmlEngine_.rootContext()->setContextProperty("$window", this);

    appQmlEngine_.initView(*this);

    connect(this, &QWindow::windowStateChanged, this, [this] { updateWindowState(); });

    updateWindowState();
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

void AppQuickView::updateWindowState()
{
    Qt::WindowStates states = windowStates();
    QQuickItem *window = rootObject();

    AppQmlEngine::updateWindowState(window, states);
}

}
