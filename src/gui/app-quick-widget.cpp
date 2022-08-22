#include "app-quick-widget.h"


#include "app-qml-engine.h"


namespace myapp {

AppQuickWidget::AppQuickWidget(AppQmlEngine &appQmlEngine, QWidget *parent)
    : QQuickWidget(&appQmlEngine, parent)
    , appQmlEngine_(appQmlEngine)
{
    setMinimumSize(QSize(800, 600));

    appQmlEngine_.initView(*this);

    updateWindowState();
}

AppQuickWidget::~AppQuickWidget()
{
}

void AppQuickWidget::keyReleaseEvent(QKeyEvent *e)
{
    if (appQmlEngine_.handleKeyRelease(e)) {
        return;
    } else {
        AppQuickWidget::keyReleaseEvent(e);
    }
}

void AppQuickWidget::changeEvent(QEvent *e)
{
    QQuickWidget::changeEvent(e);

    switch (e->type()) {
    case QEvent::Type::WindowStateChange:
        updateWindowState();
        break;
    default:
        break;
    }
}

void AppQuickWidget::updateWindowState()
{
    Qt::WindowStates states = windowState();
    QQuickItem *window = rootObject();

    AppQmlEngine::updateWindowState(window, states);
}

}
