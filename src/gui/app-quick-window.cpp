#include "app-quick-window.h"

#include <QQuickItem>

#include "app-qml-engine.h"
#include "app-quick-widget.h"


namespace myapp {

AppQuickWindow::AppQuickWindow(AppQmlEngine &appQmlEngine, QWidget *parent)
    : FramelessWindow(parent)
    , appQmlEngine_(appQmlEngine)
{
    appWidget_ = new AppQuickWidget(appQmlEngine_, *this, this);

    setCentralWidget(appWidget_);

    updateWindowState();
}

bool AppQuickWindow::hasWindowMaxButton() const
{
    return appWidget_->windowMaxButton();
}

void AppQuickWindow::setWindowMaxButtonHover(bool hover)
{
    if (QQuickItem *button = appWidget_->windowMaxButton()) {
        button->setProperty("mouseOverByFramelessWindow", hover);
    }
}

void AppQuickWindow::onWindowMaxButtonClicked()
{
    if (isMaximized()) {
        showNormal();
    } else {
        showMaximized();
    }
}

bool AppQuickWindow::isWindowMaxButtonContainsGlobalPos(const QPoint &gpos) const
{
    if (QQuickItem *button = appWidget_->windowMaxButton()) {
        const QPointF gposF{ qreal(gpos.x()), qreal(gpos.y()) };
        const QPointF lposF = button->mapFromGlobal(gposF);
        return button->contains(lposF);
    }
    return false;
}

bool AppQuickWindow::isWindowMoveAreaContainsGlobalPos(const QPoint &gpos) const
{

    if (QQuickItem *titleBar = appWidget_->windowTitleBar()) {
        const QPointF gposF{ qreal(gpos.x()), qreal(gpos.y()) };
        const QPointF lposF = titleBar->mapFromGlobal(gposF);
        if (titleBar->contains(lposF)) {
            if (QQuickItem *windowStateSwitcher = appWidget_->windowStateSwitcher()) {
                if (windowStateSwitcher->contains(windowStateSwitcher->mapFromGlobal(gposF))) {
                    return false;
                }
            }
            return true;
        } else {
            return false;
        }
    }
    return false;
}

void AppQuickWindow::keyReleaseEvent(QKeyEvent *e)
{
    if (appQmlEngine_.handleKeyRelease(e)) {
        return;
    } else {
        FramelessWindow::keyReleaseEvent(e);
    }
}

void AppQuickWindow::changeEvent(QEvent *e)
{
    FramelessWindow::changeEvent(e);

    switch (e->type()) {
    case QEvent::Type::WindowStateChange:
        updateWindowState();
        break;
    default:
        break;
    }
}

void AppQuickWindow::updateWindowState()
{
    appWidget_->updateWindowState(windowState());
}

}
