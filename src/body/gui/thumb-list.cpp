#include "thumb-list.h"

#include "book/book.h"
#include "thumb-item.h"


namespace myapp {

static constexpr int widthHint = 200;

ThumbList::ThumbList(Book &book, QWidget *parent)
    : QScrollArea(parent)
    , book_(book)
{
    setHorizontalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOn);

    setMinimumWidth(widthHint);
    //setMaximumWidth(widthHint * 2);

    jumpBar_ = new JumpBar(this);
    setVerticalScrollBar(jumpBar_);

    layout_ = new QVBoxLayout(root_);

    root_ = new QWidget(this);
    root_->setLayout(layout_);

    setWidget(root_);

    setWidgetResizable(true);

    connect(&book_, &Book::sigPageLoaded, this, [this](const QString &entryPath, i32 width, i32 height) {
        addPageThumbnailItemWidget(entryPath, width, height);
    });
}

void ThumbList::addPageThumbnailItemWidget(const QString &entryPath, i32 width, i32 height)
{
    ThumbItem *itemWidget = new ThumbItem(entryPath.toStdU32String(), width, height, this);
    layout_->addWidget(itemWidget);
    root_->adjustSize();
}

void ThumbList::resizeEvent(QResizeEvent *)
{
    root_->adjustSize();
}


}
