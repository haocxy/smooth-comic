#include "thumb-img.h"

#include <QPainter>

#include "core/logger.h"


using namespace logger::global_loggers;

namespace myapp {

ThumbImg::ThumbImg(PageNum pageNum, const QPixmap &img, QWidget *parent)
    : QWidget(parent)
    , pageNum_(pageNum)
    , img_(img)
{
    QSizePolicy sp(sizePolicy());
    sp.setHeightForWidth(true);
    setSizePolicy(sp);
}

void ThumbImg::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.drawPixmap(rect(), img_);
}

}
