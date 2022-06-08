#include "thumb-img.h"

#include <QPainter>

#include "core/logger.h"


namespace myapp {

using logger::gLogger;

ThumbImg::ThumbImg(const u32str &entryPath, i32 width, i32 height, QWidget *parent)
    : QWidget(parent)
    , entryPath_(entryPath)
    , width_(width)
    , height_(height)
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
    p.fillRect(rect(), Qt::lightGray);
    //p.drawPixmap(rect(), img_);
}

}
