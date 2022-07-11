#include "page-sprite.h"

#include <QPainter>

#include "gui-util/painter-saver.h"


namespace myapp {

static QSize calcRotatedSize(const QSize &rawSize, PageDirection dir)
{
    switch (dir) {
    case PageDirection::Up:
    case PageDirection::Down:
        return rawSize;
    case PageDirection::Left:
    case PageDirection::Right:
        return QSize(rawSize.height(), rawSize.width());
    default:
        throw std::logic_error("calcRotatedSize() bad enum");
    }
}

static int toDegree(PageDirection dir)
{
    switch (dir) {
    case PageDirection::Up:
        return 0;
    case PageDirection::Down:
        return 180;
    case PageDirection::Left:
        return 270;
    case PageDirection::Right:
        return 90;
    default:
        throw std::logic_error("cvtDirEnumToDegree() bad enum");
    }
}

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
    PainterSaver saver(painter);

    const_cast<PageSprite *>(this)->updateMatrix();

    qreal w = rawImg_.width();
    qreal h = rawImg_.height();

    qreal ox = pos_.x();
    qreal oy = pos_.y();

    qreal x = -w * anchor_.x();
    qreal y = -h * anchor_.y();

    painter.translate(ox, oy);

    painter.setTransform(matrix_);

    painter.drawPixmap(QRect(x, y, w, h), rawImg_);
}

void PageSprite::move(const QPointF &pos)
{
    pos_ = pos;
    dirty_ = true;
}

void PageSprite::rotate(PageDirection direction)
{
    dir_ = direction;
    dirty_ = true;
}

void PageSprite::updateMatrix() const
{
    if (!dirty_) {
        return;
    }

    matrix_.reset();

    matrix_.rotate(toDegree(dir_));
    matrix_.scale(scale_, scale_);
    matrix_.translate(pos_.x(), pos_.y());

    dirty_ = false;
}

}
