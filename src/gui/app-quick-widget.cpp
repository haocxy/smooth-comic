#include "app-quick-widget.h"

#include <QQuickItem>
#include <QQmlContext>

#include "app-qml-engine.h"


namespace myapp {

AppQuickWidget::AppQuickWidget(AppQmlEngine &appQmlEngine, QWidget &realWindow, QWidget *parent)
    : QQuickWidget(&appQmlEngine, parent)
    , appQmlEngine_(appQmlEngine)
{
    setMinimumSize(QSize(800, 600));

    // 因为 AppQuickWidget 不是直接作为窗口, 从QML中通过 Window.window 获取到的窗口不是真正的窗口
    appQmlEngine_.rootContext()->setContextProperty("$window", &realWindow);

    appQmlEngine_.initView(*this);

    windowTitleBar_ = rootObject()->findChild<QQuickItem *>(
        "windowTitleBar", Qt::FindChildrenRecursively);

    windowMaxButton_ = rootObject()->findChild<QQuickItem *>(
        "windowMaxButton", Qt::FindChildrenRecursively);

    windowStateSwitcher_ = rootObject()->findChild<QQuickItem *>(
        "windowStateSwitcher", Qt::FindChildrenRecursively);
}

AppQuickWidget::~AppQuickWidget()
{
    appQmlEngine_.rootContext()->setContextProperty("$window", this);
}

void AppQuickWidget::updateWindowState(const Qt::WindowStates &states)
{
    QQuickItem *window = rootObject();
    AppQmlEngine::updateWindowState(window, states);
}

}
