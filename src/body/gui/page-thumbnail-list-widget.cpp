#include "page-thumbnail-list-widget.h"

#include "book/book.h"
#include "thumb-item.h"


namespace myapp {

static constexpr int widthHint = 200;

PageThumbnailListWidget::PageThumbnailListWidget(Book &book, QWidget *parent)
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

    connect(&book_, &Book::sigPageLoaded, this, [this](PageNum pageNum, const QPixmap &img, PageNum totalPages) {
        addPageThumbnailItemWidget(pageNum, img);
    });
}

void PageThumbnailListWidget::addPageThumbnailItemWidget(PageNum pageNum, const QPixmap &img)
{
    ThumbItem *itemWidget = new ThumbItem(pageNum, img, QString::number(pageNum), this);
    layout_->addWidget(itemWidget);
    root_->adjustSize();
}

void PageThumbnailListWidget::resizeEvent(QResizeEvent *)
{
    root_->adjustSize();
}


}
