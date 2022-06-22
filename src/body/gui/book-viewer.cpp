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
#include "thumb-list-scroll-area.h"


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
    initThumbArea();
    initPageViewerArea();
    initStatusArea();
}

void BookViewer::initMenuBarArea()
{
    using Class = BookViewer;

    QMenuBar *bar = menuBar();

    QMenu *fileMenu = bar->addMenu(tr("File"));
    bind(fileMenu, tr("Open"), &Class::fileOpenAction, QKeySequence::StandardKey::Open);

    QMenu *viewMenu = bar->addMenu(tr("View"));
}

void BookViewer::initToolBarArea()
{
    using Class = BookViewer;

    QToolBar *fileBar = addToolBar(tr("File"));
    bind(fileBar, tr("Open"), &Class::fileOpenAction);
}

void BookViewer::initThumbArea()
{
    thumbList_ = new ThumbListScrollArea(*book_, this);
    QDockWidget *thumbDock = new QDockWidget(tr("Thumbnail Area"), this);
    thumbDock->setAllowedAreas(Qt::DockWidgetArea::LeftDockWidgetArea | Qt::DockWidgetArea::RightDockWidgetArea);
    thumbDock->setWidget(thumbList_);
    addDockWidget(Qt::DockWidgetArea::LeftDockWidgetArea, thumbDock);
}

void BookViewer::initPageViewerArea()
{
    pageViewer_ = new PageViewerWidget(*book_, this);
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

}
