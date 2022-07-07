#include "app-window.h"

#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QToolBar>
#include <QDockWidget>
#include <QStatusBar>
#include <QFileDialog>

#include "core/system.h"
#include "core/logger.h"

#include "title-bar-area/title-bar-area.h"
#include "book-area/book-area.h"


namespace myapp {

using logger::gLogger;

AppWindow::AppWindow(Engine &engine, QWidget *parent)
    : FramelessWindow(parent)
    , engine_(engine)
{
    book_ = std::make_unique<Book>(engine_);

    resize(1000, 800);
    setMinimumSize(800, 600);

    initAreas();
}

TitleBarButton *AppWindow::windowMaxButton()
{
    return titleBarArea_->windowMaxButton();
}

bool AppWindow::isWindowMaxButtonContainsGlobalPos(const QPoint &gpos) const
{
    TitleBarButton *b = titleBarArea_->windowMaxButton();
    return b->rect().contains(b->mapFromGlobal(gpos));
}

bool AppWindow::isWindowMoveAreaContainsGlobalPos(const QPoint &gpos) const
{
    return titleBarArea_->isWindowMoveAreaContainsGlobalPos(gpos);
}

void AppWindow::initAreas()
{
    rootWidget_ = new QWidget(this);
    rootWidget_->setContentsMargins(QMargins());
    setCentralWidget(rootWidget_);

    rootLayout_ = new QVBoxLayout(rootWidget_);
    rootLayout_->setContentsMargins(QMargins());
    rootWidget_->setLayout(rootLayout_);

    initTitleBarArea();

    initBookArea();
}

void AppWindow::initTitleBarArea()
{
    titleBarArea_ = new TitleBarArea(this);
    rootLayout_->addWidget(titleBarArea_);

    connect(titleBarArea_->windowMaxButton(), &QPushButton::clicked, this, &AppWindow::toggleWindowMaxAction);
    connect(titleBarArea_->windowCloseButton(), &QPushButton::clicked, this, &AppWindow::close);
}

void AppWindow::initBookArea()
{
    bookArea_ = new BookArea(*book_, this);
    rootLayout_->addWidget(bookArea_);
}

void AppWindow::bind(QMenu *menu, const QString &name, void(AppWindow:: *f)())
{
    QAction *action = menu->addAction(name);
    connect(action, &QAction::triggered, this, f);
}

void AppWindow::bind(QMenu *menu, const QString &name, void(AppWindow:: *f)(), const QKeySequence &shortcut)
{
    QAction *action = menu->addAction(name);
    action->setShortcut(shortcut);
    connect(action, &QAction::triggered, this, f);
}

void AppWindow::bind(QToolBar *toolBar, const QString &name, void(AppWindow:: *f)())
{
    QAction *action = toolBar->addAction(name);
    connect(action, &QAction::triggered, this, f);
}

void AppWindow::toggleWindowMaxAction()
{
    if (isMaximized()) {
        showNormal();
    } else {
        showMaximized();
    }
}

void AppWindow::fileOpenAction()
{
    const fs::path home = SystemUtil::userHome();
    const QString defaultDir = QString::fromStdU32String(home.generic_u32string());
    const QString filePath = QFileDialog::getOpenFileName(this, tr("Open Comic"), defaultDir);

    book_->open(filePath.toStdU32String());
}

void AppWindow::bookCloseAction()
{
    book_->close();
}

void AppWindow::bookReloadAction()
{
    book_->reload();
}

void AppWindow::pageNextAction()
{
    bookArea_->jumpNext();
}

void AppWindow::pagePrevAction()
{
    bookArea_->jumpPrev();
}

}
