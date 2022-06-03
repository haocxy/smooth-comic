#include "book-viewer-widget.h"

#include <QDockWidget>
#include <QStatusBar>

#include "artboard-widget.h"
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
    resize(1000, 800);
    setMinimumSize(800, 600);

    thumbnailList_ = new PageThumbnailListWidget(this);
    //setHStreachFactor(thumbnailList_, 1);
    //addWidget(thumbnailList_);
    QDockWidget *thumbDock = new QDockWidget(tr("Thumbnail Area"), this);
    thumbDock->setAllowedAreas(Qt::DockWidgetArea::LeftDockWidgetArea | Qt::DockWidgetArea::RightDockWidgetArea);
    thumbDock->setWidget(thumbnailList_);
    addDockWidget(Qt::DockWidgetArea::LeftDockWidgetArea, thumbDock);

    artboard_ = new ArtboardWidget(this);
    //setHStreachFactor(artboard_, 9);
    //addWidget(artboard_);
    setCentralWidget(artboard_);

    statusBar_ = new BookViewerStatusBarWidget(this);
    statusBar()->addWidget(statusBar_);

    book_ = new Book(mEngine, this);

    connect(book_, &Book::sigPageLoaded, this, [this](PageNum pageNum, const QPixmap &img) {
        thumbnailList_->addPageThumbnailItemWidget(pageNum, img);
    });
}

}
