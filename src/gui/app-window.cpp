#include "app-window.h"

#include <QEvent>
#include <QWindowStateChangeEvent>

#include "core/logger.h"

#include "title-bar-area/title-bar-area.h"

#include "app-widget.h"


namespace myapp {

using logger::gLogger;

AppWindow::AppWindow(Engine &engine, QWidget *parent)
    : FramelessWindow(parent)
{
    appWidget_ = new AppWidget(engine, this);

    qDebug() << "after new AppWidget()";

    setCentralWidget(appWidget_);

    if (TitleBarArea *bar = appWidget_->titleBarArea()) {
        connect(bar->windowFullScreenButton(), &QPushButton::clicked, this, &AppWindow::toggleWindowFullScreenAction);
        connect(bar->windowMinButton(), &QPushButton::clicked, this, &AppWindow::showMinimized);
        connect(bar->windowMaxButton(), &QPushButton::clicked, this, &AppWindow::toggleWindowMaxAction);
        connect(bar->windowCloseButton(), &QPushButton::clicked, this, &AppWindow::close);
    }
}

TitleBarButton *AppWindow::windowMaxButton()
{
    qDebug() << __FUNCTION__;

    if (TitleBarArea *bar = appWidget_->titleBarArea()) {
        return bar->windowMaxButton();
    } else {
        return nullptr;
    }
}

bool AppWindow::isWindowMaxButtonContainsGlobalPos(const QPoint &gpos) const
{
    qDebug() << __FUNCTION__;
    if (TitleBarArea *bar = appWidget_->titleBarArea()) {
        TitleBarButton *btn = bar->windowMaxButton();
        return btn->rect().contains(btn->mapFromGlobal(gpos));
    } else {
        return false;
    }
}

bool AppWindow::isWindowMoveAreaContainsGlobalPos(const QPoint &gpos) const
{
    qDebug() << __FUNCTION__;
    return appWidget_->isWindowMoveAreaContainsGlobalPos(gpos);
}

void AppWindow::changeEvent(QEvent *e)
{
    qDebug() << __FUNCTION__;
    FramelessWindow::changeEvent(e);

    qDebug() << __FUNCTION__ << " after FramelessWindow::changeEvent";

    switch (e->type()) {
    case QEvent::Type::WindowStateChange:
        qDebug() << __FUNCTION__ << " before if";
        if (QWindowStateChangeEvent *stateChangeEvent = dynamic_cast<QWindowStateChangeEvent *>(e)) {
            qDebug() << __FUNCTION__ << " before call";
            onWindowStateChanged(stateChangeEvent->oldState(), windowState());
            qDebug() << __FUNCTION__ << " after call";
        }
        break;
    default:
        break;
    }
}

void AppWindow::toggleWindowFullScreenAction()
{
    qDebug() << __FUNCTION__;

    using s = Qt::WindowState;

    const Qt::WindowStates states = windowState();

    if (states.testFlag(s::WindowFullScreen)) {
        switchWindowStateTo(s::WindowNoState);
    } else if (states.testFlag(s::WindowMaximized)) {
        switchWindowStateTo(s::WindowFullScreen);
    } else if (states.testFlag(s::WindowNoState)) {
        switchWindowStateTo(s::WindowFullScreen);
    }
}

void AppWindow::toggleWindowMaxAction()
{
    qDebug() << __FUNCTION__;
    using s = Qt::WindowState;

    const Qt::WindowStates states = windowState();

    if (states.testFlag(s::WindowFullScreen)) {
        switchWindowStateTo(s::WindowMaximized);
    } else if (states.testFlag(s::WindowMaximized)) {
        switchWindowStateTo(s::WindowNoState);
    } else if (states.testFlag(s::WindowNoState)) {
        switchWindowStateTo(s::WindowMaximized);
    }
}

void AppWindow::switchWindowStateTo(Qt::WindowState newState)
{
    qDebug() << __FUNCTION__;
    switch (newState) {
    case Qt::WindowState::WindowFullScreen:
        showFullScreen();
        break;
    case Qt::WindowState::WindowMaximized:
        showMaximized();
        break;
    case Qt::WindowState::WindowMinimized:
        showMinimized();
        break;
    case Qt::WindowState::WindowNoState:
        showNormal();
        break;
    default:
        break;
    }
}

void AppWindow::onWindowStateChanged(Qt::WindowStates oldStates, Qt::WindowStates newStates)
{
    qDebug() << __FUNCTION__;
    if (TitleBarArea *bar = appWidget_->titleBarArea()) {
        using s = Qt::WindowState;
        using f = FontIconEnum;
        if (newStates.testFlag(s::WindowFullScreen)) {
            bar->windowFullScreenButton()->setFontIcon(f::WindowBackToWindow);
            bar->windowMaxButton()->setFontIcon(f::WindowMaximize);
        } else if (newStates.testFlag(s::WindowMaximized)) {
            bar->windowFullScreenButton()->setFontIcon(f::WindowFullScreen);
            bar->windowMaxButton()->setFontIcon(f::WindowRestore);
        } else if (newStates.testFlag(s::WindowNoState)) {
            bar->windowFullScreenButton()->setFontIcon(f::WindowFullScreen);
            bar->windowMaxButton()->setFontIcon(f::WindowMaximize);
        }
        qDebug() << __FUNCTION__ << "b";

    }
}

}
