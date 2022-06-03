#include "page-thumbnail-list-widget.h"

#include "page-thumbnail-item-widget.h"


namespace myapp {

static constexpr int widthHint = 200;

PageThumbnailListWidget::PageThumbnailListWidget(QWidget *parent)
    : QScrollArea(parent)
{
    setHorizontalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOn);

    setMinimumWidth(widthHint);
    setMaximumWidth(widthHint * 2);

    layout_ = new QVBoxLayout(root_);

    root_ = new QWidget(this);
    root_->setLayout(layout_);

    setWidget(root_);

    setWidgetResizable(true);
}

void PageThumbnailListWidget::addPageThumbnailItemWidget(PageNum pageNum, const QPixmap &img)
{
    PageThumbnailItemWidget *itemWidget = new PageThumbnailItemWidget(pageNum, img, QString::number(pageNum), this);
    layout_->addWidget(itemWidget);
}

void PageThumbnailListWidget::resizeEvent(QResizeEvent *)
{
    root_->resize(childrenRect().width(), root_->height());
}


}
