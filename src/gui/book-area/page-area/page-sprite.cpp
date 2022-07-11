#include "page-sprite.h"

#include <QPainter>


namespace myapp {

PageSprite::PageSprite(const QPixmap &rawImg, QObject *parent)
    : QObject(parent)
    , rawImg_(rawImg)
{
    showSize_ = rawImg.size();
}

PageSprite::~PageSprite()
{
}

void PageSprite::draw(QPainter &painter) const
{
    painter.drawPixmap(QRect({}, showSize_), rawImg_);
}

}
