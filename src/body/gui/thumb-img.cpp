#include "thumb-img.h"

#include <QPainter>

#include "core/logger.h"


namespace myapp {

using logger::gLogger;

ThumbImg::ThumbImg(PageNum pageNum, const QPixmap &img, QWidget *parent)
    : QWidget(parent)
    , pageNum_(pageNum)
    , img_(img)
{
    QSizePolicy spImg(sizePolicy());
    spImg.setVerticalPolicy(QSizePolicy::Policy::Ignored);
    spImg.setHorizontalPolicy(QSizePolicy::Policy::Expanding);
    spImg.setHeightForWidth(true);
    setSizePolicy(spImg);
}

void ThumbImg::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.drawPixmap(rect(), img_);
}

}
