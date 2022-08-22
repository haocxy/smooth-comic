#include "app-quick-widget.h"


#include "app-qml-engine.h"


namespace myapp {

AppQuickWidget::AppQuickWidget(AppQmlEngine &appQmlEngine, QWidget *parent)
    : QQuickWidget(&appQmlEngine, parent)
    , appQmlEngine_(appQmlEngine)
{
    appQmlEngine_.initView(*this);
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

}
