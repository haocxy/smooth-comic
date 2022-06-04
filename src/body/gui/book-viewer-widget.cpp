#include "book-viewer.h"

#include <QDockWidget>
#include <QStatusBar>

#include "page-viewer-widget.h"
#include "book-viewer-status-bar-widget.h"



namespace myapp {

static void setHStreachFactor(QWidget *w, int strechFactor)
{
    QSizePolicy p = w->sizePolicy();
    p.setHorizontalStretch(strechFactor);
    w->setSizePolicy(p);
}

BookViewerWidget::BookViewerWidget(Engine &engine, QWidget *parent)
    : QMainWindow(parent)
    , mEngine(engine)
{
    book_ = new Book(mEngine, this);

    resize(1000, 800);
    setMinimumSize(800, 600);

    thumbnailList_ = new PageThumbnailListWidget(*book_, this);
    QDockWidget *thumbDock = new QDockWidget(tr("Thumbnail Area"), this);
    thumbDock->setAllowedAreas(Qt::DockWidgetArea::LeftDockWidgetArea | Qt::DockWidgetArea::RightDockWidgetArea);
    thumbDock->setWidget(thumbnailList_);
    addDockWidget(Qt::DockWidgetArea::LeftDockWidgetArea, thumbDock);

    pageViewer_ = new PageViewerWidget(this);
    setCentralWidget(pageViewer_);

    statusBar_ = new BookViewerStatusBarWidget(*book_, this);
    statusBar()->addWidget(statusBar_);
}

}
