#include "thumb-img-widget.h"

#include <QPainter>

#include "core/logger.h"


using namespace logger::global_loggers;

namespace myapp {

ThumbImgWidget::ThumbImgWidget(PageNum pageNum, const QPixmap &img, QWidget *parent)
    : QWidget(parent)
    , pageNum_(pageNum)
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
