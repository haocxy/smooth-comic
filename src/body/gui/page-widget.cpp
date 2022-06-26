#include "page-widget.h"

#include <QPainter>


namespace myapp {

PageWidget::PageWidget(Book &book, PageNum seqNum, const QPixmap &img, QWidget *parent)
    : QWidget(parent)
    , book_(book)
    , seqNum_(seqNum)
    , img_(img)
{
}

PageWidget::~PageWidget()
{
}

void PageWidget::paintEvent(QPaintEvent *e)
{
    QPainter p(this);

    p.fillRect(rect(), Qt::gray);
    p.drawPixmap(rect(), img_);
}

}
