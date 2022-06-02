#include "thumb-img-widget.h"

#include <QPainter>


namespace myapp {

ThumbImgWidget::ThumbImgWidget(const QPixmap &img, QWidget *parent)
    : QWidget(parent)
    , img_(img)
{
}

void ThumbImgWidget::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    //QPixmap scaledImg = img_.scaledToWidth(width()); // TODO 临时写法
    //p.drawPixmap(0, 0, scaledImg);
    p.drawLine(0, 0, width(), height());
}

}
