#include "main-window.h"

#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QToolBar>
#include <QDockWidget>
#include <QStatusBar>
#include <QFileDialog>

#include "core/system.h"
#include "core/logger.h"

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
    return windowMaxButton_;
}

bool MainWindow::isWindowMaxButtonContainsGlobalPos(const QPoint &gpos) const
{
    return windowMaxButton_->rect().contains(windowMaxButton_->mapFromGlobal(gpos));
}

bool MainWindow::isWindowMoveAreaContainsGlobalPos(const QPoint &gpos) const
{
    QMenuBar *menuArea = menuBar();

    const QPoint menuLocalPos = menuArea->mapFromGlobal(gpos);

    if (!menuArea->rect().contains(menuLocalPos)) {
        return false;
    }

    if (menuArea->actionAt(menuLocalPos)) {
        return false;
    }

    return true;
}

void MainWindow::initAreas()
{
    initMenuBarArea();
    initToolBarArea();
    initPageViewerArea();
    initStatusArea();
}

void MainWindow::initMenuBarArea()
{
    using Class = MainWindow;

    QMenuBar *bar = menuBar();

    QMenu *fileMenu = bar->addMenu(tr("File"));
    bind(fileMenu, tr("Open"), &Class::fileOpenAction, QKeySequence::StandardKey::Open);
    bind(fileMenu, tr("Close"), &Class::bookCloseAction, QKeySequence::StandardKey::Close);
    bind(fileMenu, tr("Reload"), &Class::bookReloadAction, QKeySequence::StandardKey::Refresh);

    QMenu *viewMenu = bar->addMenu(tr("View"));
    bind(viewMenu, tr("Next Page"), &Class::pageNextAction);
    bind(viewMenu, tr("Prev Page"), &Class::pagePrevAction);
}

void MainWindow::initToolBarArea()
{
    using Class = MainWindow;

    QToolBar *fileBar = addToolBar(tr("File"));
    bind(fileBar, tr("Open"), &Class::fileOpenAction);
    bind(fileBar, tr("Close"), &Class::bookCloseAction);
    bind(fileBar, tr("Reload"), &Class::bookReloadAction);

    QToolBar *viewBar = addToolBar(tr("View"));
    bind(viewBar, tr("Next Page"), &Class::pageNextAction);
    bind(viewBar, tr("Prev Page"), &Class::pagePrevAction);

    windowMaxButton_ = new TitleBarButton("Max Window");
    viewBar->addWidget(windowMaxButton_);

    connect(windowMaxButton_, &QPushButton::clicked, this, [this] {
        if (isMaximized()) {
            showNormal();
        } else {
            showMaximized();
        }
    });
}

void MainWindow::initPageViewerArea()
{
    bookArea_ = new BookArea(*book_, this);
    setCentralWidget(bookArea_);
}

void MainWindow::initStatusArea()
{
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
