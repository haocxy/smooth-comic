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

MainWindow::MainWindow(Engine &engine, QWidget *parent)
    : FramelessWindow(parent)
    , engine_(engine)
{
    book_ = std::make_unique<Book>(engine_);

    resize(1000, 800);
    setMinimumSize(800, 600);

    initAreas();
}

TitleBarButton *MainWindow::windowMaxButton()
{
    return titleBarArea_->windowMaxButton();
}

bool MainWindow::isWindowMaxButtonContainsGlobalPos(const QPoint &gpos) const
{
    TitleBarButton *b = titleBarArea_->windowMaxButton();
    return b->rect().contains(b->mapFromGlobal(gpos));
}

bool MainWindow::isWindowMoveAreaContainsGlobalPos(const QPoint &gpos) const
{
    return titleBarArea_->isWindowMoveAreaContainsGlobalPos(gpos);
}

void MainWindow::initAreas()
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

void MainWindow::initTitleBarArea()
{
    titleBarArea_ = new TitleBarArea(this);
    rootLayout_->addWidget(titleBarArea_);

    connect(titleBarArea_->windowMaxButton(), &QPushButton::clicked, this, &MainWindow::toggleWindowMaxAction);
    connect(titleBarArea_->windowCloseButton(), &QPushButton::clicked, this, &MainWindow::close);
}

void MainWindow::initBookArea()
{
    bookArea_ = new BookArea(*book_, this);
    rootLayout_->addWidget(bookArea_);
}

void MainWindow::bind(QMenu *menu, const QString &name, void(MainWindow:: *f)())
{
    QAction *action = menu->addAction(name);
    connect(action, &QAction::triggered, this, f);
}

void MainWindow::bind(QMenu *menu, const QString &name, void(MainWindow:: *f)(), const QKeySequence &shortcut)
{
    QAction *action = menu->addAction(name);
    action->setShortcut(shortcut);
    connect(action, &QAction::triggered, this, f);
}

void MainWindow::bind(QToolBar *toolBar, const QString &name, void(MainWindow:: *f)())
{
    QAction *action = toolBar->addAction(name);
    connect(action, &QAction::triggered, this, f);
}

void MainWindow::toggleWindowMaxAction()
{
    if (isMaximized()) {
        showNormal();
    } else {
        showMaximized();
    }
}

void MainWindow::fileOpenAction()
{
    const fs::path home = SystemUtil::userHome();
    const QString defaultDir = QString::fromStdU32String(home.generic_u32string());
    const QString filePath = QFileDialog::getOpenFileName(this, tr("Open Comic"), defaultDir);

    book_->open(filePath.toStdU32String());
}

void MainWindow::bookCloseAction()
{
    book_->close();
}

void MainWindow::bookReloadAction()
{
    book_->reload();
}

void MainWindow::pageNextAction()
{
    bookArea_->jumpNext();
}

void MainWindow::pagePrevAction()
{
    bookArea_->jumpPrev();
}

}
