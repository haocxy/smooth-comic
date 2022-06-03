#include "thumb-img-widget.h"

#include <QPainter>


namespace myapp {

ThumbImgWidget::ThumbImgWidget(const QPixmap &img, QWidget *parent)
    : QWidget(parent)
    , img_(img)
{
    QSizePolicy sp(sizePolicy());
    sp.setHeightForWidth(true);
    setSizePolicy(sp);
}

void ThumbImgWidget::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.drawPixmap(rect(), img_);
}

}
