#include "page-thumbnail-list-widget.h"


namespace myapp {

static constexpr int widthHint = 200;

PageThumbnailListWidget::PageThumbnailListWidget(QWidget *parent)
    : QScrollArea(parent)
{
    setHorizontalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOn);

    setMinimumWidth(widthHint);
    setMaximumWidth(widthHint * 2);
}


}
