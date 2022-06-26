#include "book-viewer.h"

#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QToolBar>
#include <QDockWidget>
#include <QStatusBar>
#include <QFileDialog>

#include "core/system.h"
#include "core/logger.h"

#include "page-viewer.h"
#include "book-status.h"


namespace myapp {

using logger::gLogger;

BookViewer::BookViewer(Engine &engine, QWidget *parent)
    : QMainWindow(parent)
    , engine_(engine)
{
    book_ = std::make_unique<Book>(engine_);

    resize(1000, 800);
    setMinimumSize(800, 600);

    initAreas();
}

void BookViewer::initAreas()
{
    initMenuBarArea();
    initToolBarArea();
    initPageViewerArea();
    initStatusArea();
}

void BookViewer::initMenuBarArea()
{
    using Class = BookViewer;

    QMenuBar *bar = menuBar();

    QMenu *fileMenu = bar->addMenu(tr("File"));
    bind(fileMenu, tr("Open"), &Class::fileOpenAction, QKeySequence::StandardKey::Open);
    bind(fileMenu, tr("Close"), &Class::bookCloseAction, QKeySequence::StandardKey::Close);
    bind(fileMenu, tr("Reload"), &Class::bookReloadAction, QKeySequence::StandardKey::Refresh);

    QMenu *viewMenu = bar->addMenu(tr("View"));
    bind(viewMenu, tr("Next Page"), &Class::pageNextAction);
    bind(viewMenu, tr("Prev Page"), &Class::pagePrevAction);
}

void BookViewer::initToolBarArea()
{
    using Class = BookViewer;

    QToolBar *fileBar = addToolBar(tr("File"));
    bind(fileBar, tr("Open"), &Class::fileOpenAction);
    bind(fileBar, tr("Close"), &Class::bookCloseAction);
    bind(fileBar, tr("Reload"), &Class::bookReloadAction);

    QToolBar *viewBar = addToolBar(tr("View"));
    bind(viewBar, tr("Next Page"), &Class::pageNextAction);
    bind(viewBar, tr("Prev Page"), &Class::pagePrevAction);
}

void BookViewer::initPageViewerArea()
{
    pageViewer_ = new PageViewer(*book_, this);
    setCentralWidget(pageViewer_);
}

void BookViewer::initStatusArea()
{
    QStatusBar *statusBar = this->statusBar();
    bookStatus_ = new BookStatus(*book_, this);
    statusBar->addWidget(bookStatus_);
}

void BookViewer::bind(QMenu *menu, const QString &name, void(BookViewer:: *f)())
{
    QAction *action = menu->addAction(name);
    connect(action, &QAction::triggered, this, f);
}

void BookViewer::bind(QMenu *menu, const QString &name, void(BookViewer:: *f)(), const QKeySequence &shortcut)
{
    QAction *action = menu->addAction(name);
    action->setShortcut(shortcut);
    connect(action, &QAction::triggered, this, f);
}

void BookViewer::bind(QToolBar *toolBar, const QString &name, void(BookViewer:: *f)())
{
    QAction *action = toolBar->addAction(name);
    connect(action, &QAction::triggered, this, f);
}

void BookViewer::fileOpenAction()
{
    const fs::path home = SystemUtil::userHome();
    const QString defaultDir = QString::fromStdU32String(home.generic_u32string());
    const QString filePath = QFileDialog::getOpenFileName(this, tr("Open Comic"), defaultDir);

    book_->open(filePath.toStdU32String());
}

void BookViewer::bookCloseAction()
{
    book_->close();
}

void BookViewer::bookReloadAction()
{
    book_->reload();
}

void BookViewer::pageNextAction()
{
    pageViewer_->jumpNext();
}

void BookViewer::pagePrevAction()
{
    pageViewer_->jumpPrev();
}

}
