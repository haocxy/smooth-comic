#include "book-viewer-widget.h"

#include "artboard-widget.h"



namespace myapp {

static void setHStreachFactor(QWidget *w, int strechFactor)
{
    QSizePolicy p = w->sizePolicy();
    p.setHorizontalStretch(strechFactor);
    w->setSizePolicy(p);
}

BookViewerWidget::BookViewerWidget(Engine &engine, QWidget *parent)
    : QSplitter(parent)
    , mEngine(engine)
{
    resize(1000, 800);
    setMinimumSize(800, 600);

    thumbnailList_ = new PageThumbnailListWidget(this);
    setHStreachFactor(thumbnailList_, 1);
    addWidget(thumbnailList_);

    artboard_ = new ArtboardWidget(this);
    setHStreachFactor(artboard_, 9);
    addWidget(artboard_);

    book_ = new Book(mEngine, this);
}

}
