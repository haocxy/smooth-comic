#include "page-sprite.h"

#include <QPainter>


namespace myapp {

PageSprite::PageSprite(const QPixmap &rawImg, QObject *parent)
    : QObject(parent)
    , rawImg_(rawImg)
{
    updateMatrix();
}

PageSprite::~PageSprite()
{
}

void PageSprite::draw(QPainter &painter) const
{
    qreal w = rawImg_.width();
    qreal h = rawImg_.height();

    qreal ox = pos_.x();
    qreal oy = pos_.y();

    qreal x = -w * anchor_.x();
    qreal y = -h * anchor_.y();

    painter.translate(ox, oy);

    painter.drawPixmap(QRect(x, y, w, h), rawImg_);
}

void PageSprite::updateMatrix()
{
    matrix_.reset();




}

}
