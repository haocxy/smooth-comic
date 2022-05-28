#include "book-viewer-widget.h"

#include "artboard-widget.h"



namespace myapp {

BookViewerWidget::BookViewerWidget(Engine &engine, QWidget *parent)
    : QWidget(parent)
    , mEngine(engine)
{


    resize(1000, 800);
    setMinimumSize(800, 600);

    mLayout = new QStackedLayout(this);
    setLayout(mLayout);

    mArtboard = new ArtboardWidget(this);
    mLayout->addWidget(mArtboard);



    mBook = new Book(mEngine, this);
}

}
