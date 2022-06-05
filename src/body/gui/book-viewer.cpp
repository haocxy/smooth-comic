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

    thumbList_ = new ThumbList(*book_, this);
    QDockWidget *thumbDock = new QDockWidget(tr("Thumbnail Area"), this);
    thumbDock->setAllowedAreas(Qt::DockWidgetArea::LeftDockWidgetArea | Qt::DockWidgetArea::RightDockWidgetArea);
    thumbDock->setWidget(thumbList_);
    addDockWidget(Qt::DockWidgetArea::LeftDockWidgetArea, thumbDock);

    pageViewer_ = new PageViewerWidget(*book_, this);
    setCentralWidget(pageViewer_);

    QStatusBar *statusBar = this->statusBar();
    bookStatus_ = new BookStatus(*book_, this);
    statusBar->addWidget(bookStatus_);
}

}
