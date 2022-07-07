#include "app-window.h"

#include <QEvent>
#include <QWindowStateChangeEvent>

#include "core/logger.h"

#include "app-widget.h"


namespace myapp {

using logger::gLogger;

AppWindow::AppWindow(Engine &engine, QWidget *parent)
    : FramelessWindow(parent)
{
    appWidget_ = new AppWidget(engine, this);
    setCentralWidget(appWidget_);

    connect(appWidget_->windowFullScreenButton(), &QPushButton::clicked, this, &AppWindow::toggleWindowFullScreenAction);
    connect(appWidget_->windowMinButton(), &QPushButton::clicked, this, &AppWindow::showMinimized);
    connect(appWidget_->windowMaxButton(), &QPushButton::clicked, this, &AppWindow::toggleWindowMaxAction);
    connect(appWidget_->windowCloseButton(), &QPushButton::clicked, this, &AppWindow::close);
}

TitleBarButton *AppWindow::windowMaxButton()
{
    return appWidget_->windowMaxButton();
}

bool AppWindow::isWindowMaxButtonContainsGlobalPos(const QPoint &gpos) const
{
    TitleBarButton *b = appWidget_->windowMaxButton();
    return b->rect().contains(b->mapFromGlobal(gpos));
}

bool AppWindow::isWindowMoveAreaContainsGlobalPos(const QPoint &gpos) const
{
    return appWidget_->isWindowMoveAreaContainsGlobalPos(gpos);
}

void AppWindow::changeEvent(QEvent *e)
{
    FramelessWindow::changeEvent(e);

    switch (e->type()) {
    case QEvent::Type::WindowStateChange:
        if (QWindowStateChangeEvent *stateChangeEvent = dynamic_cast<QWindowStateChangeEvent *>(e)) {
            onWindowStateChanged(stateChangeEvent->oldState(), windowState());
        }
        break;
    default:
        break;
    }
}

void AppWindow::toggleWindowFullScreenAction()
{
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
    qDebug() << "AppWindow::onWindowStateChanged() from " << oldStates << " to " << newStates;

    if (newStates.testFlag(Qt::WindowState::WindowFullScreen)) {
        appWidget_->windowFullScreenButton()->setFontIcon(FontIconEnum::WindowBackToWindow);
        appWidget_->windowMaxButton()->setFontIcon(FontIconEnum::WindowMaximize);
    } else if (newStates.testFlag(Qt::WindowState::WindowMaximized)) {
        appWidget_->windowFullScreenButton()->setFontIcon(FontIconEnum::WindowFullScreen);
        appWidget_->windowMaxButton()->setFontIcon(FontIconEnum::WindowRestore);
    } else if (newStates.testFlag(Qt::WindowState::WindowNoState)) {
        appWidget_->windowFullScreenButton()->setFontIcon(FontIconEnum::WindowFullScreen);
        appWidget_->windowMaxButton()->setFontIcon(FontIconEnum::WindowMaximize);
    }
}

}
