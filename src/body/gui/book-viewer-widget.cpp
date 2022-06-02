#include "book-viewer-widget.h"

#include "artboard-widget.h"



namespace myapp {

BookViewerWidget::BookViewerWidget(Engine &engine, QWidget *parent)
    : QSplitter(parent)
    , mEngine(engine)
{
    resize(1000, 800);
    setMinimumSize(800, 600);

    artboard_ = new ArtboardWidget(this);
    addWidget(artboard_);

    book_ = new Book(mEngine, this);
}

}
