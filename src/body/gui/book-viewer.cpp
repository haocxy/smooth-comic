#include "book-viewer.h"

#include <QDockWidget>
#include <QStatusBar>

#include "page-viewer.h"
#include "book-status.h"
#include "thumb-list.h"


namespace myapp {


BookViewer::BookViewer(Engine &engine, QWidget *parent)
    : QMainWindow(parent)
    , engine_(engine)
{
    book_ = new Book(engine_, this);

    resize(1000, 800);
    setMinimumSize(800, 600);

    initAreas();
}

void BookViewer::initAreas()
{
    initThumbArea();
    initPageViewerArea();
    initStatusArea();
}

void BookViewer::initThumbArea()
{
    thumbList_ = new ThumbList(*book_, this);
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

}
