#include "app-window.h"

#include "core/logger.h"

#include "app-widget.h"


namespace myapp {

using logger::gLogger;

AppWindow::AppWindow(Engine &engine, QWidget *parent)
    : FramelessWindow(parent)
{
    appWidget_ = new AppWidget(engine, this);
    setCentralWidget(appWidget_);

    connect(appWidget_->windowFullScreenButton(), &QPushButton::clicked, this, &AppWindow::toggleWindowFullScreen);
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

void AppWindow::toggleWindowFullScreen()
{
    if (isFullScreen()) {
        showNormal();
    } else {
        showFullScreen();
    }
}

void AppWindow::toggleWindowMaxAction()
{
    if (isMaximized()) {
        showNormal();
    } else {
        showMaximized();
    }
}

}
