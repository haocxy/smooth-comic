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
    : QMainWindow(parent)
    , engine_(engine)
{
    book_ = std::make_unique<Book>(engine_);

    resize(1000, 800);
    setMinimumSize(800, 600);

    initAreas();
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
}

void MainWindow::initPageViewerArea()
{
    pageViewer_ = new PageViewer(*book_, this);
    setCentralWidget(pageViewer_);
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
    pageViewer_->jumpNext();
}

void MainWindow::pagePrevAction()
{
    pageViewer_->jumpPrev();
}

}
