#include "page-sprite.h"

#include <cmath>

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

PageSprite::PageSprite(PageNum seqNum, const QPixmap &rawImg, QObject *parent)
    : QObject(parent)
    , seqNum_(seqNum)
    , rawImg_(rawImg)
{
    updateMatrix();

    rotatedSize_ = calcRotatedSize(rawImg_.size(), dir_);
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

    qreal x = -w * anchor_.x();
    qreal y = -h * anchor_.y();

    painter.setTransform(matrix_);

    painter.drawPixmap(QRect(x, y, w, h), rawImg_);
}

static qreal calcScaleForAdjustAreaSize(const QSize &imgSize, const QSize &areaSize)
{
    if (imgSize.width() > 0 && areaSize.width() > 0) {
        const qreal scaleW = qreal(areaSize.width()) / imgSize.width();
        const qreal scaleH = qreal(areaSize.height()) / imgSize.height();
        return std::min(scaleW, scaleH);
    } else {
        return 0;
    }
}

float PageSprite::calcMinScale(const QSize &areaSize) const
{
    return std::min(1.0, calcScaleForAdjustAreaSize(rawImg_.size(), areaSize / 2));
}

float PageSprite::calcMaxScale(const QSize &areaSize) const
{
    return std::max(1.0, calcScaleForAdjustAreaSize(rawImg_.size(), areaSize * 2));
}

void PageSprite::adjustAreaSize(const QSize &areaSize)
{
    setScale(calcScaleForAdjustAreaSize(rawImg_.size(), areaSize));
}

void PageSprite::adjustAreaWidth(int areaWidth)
{
    if (rotatedSize_.width() > 0 && areaWidth > 0) {
        setScale(qreal(areaWidth) / rotatedSize_.width());
    }
}

void PageSprite::adjustAreaHeight(int areaHeight)
{
    if (rotatedSize_.height() > 0 && areaHeight > 0) {
        setScale(qreal(areaHeight) / rotatedSize_.height());
    }
}

void PageSprite::rotateTo(PageDirection direction)
{
    dir_ = direction;

    rotatedSize_ = calcRotatedSize(rawImg_.size(), dir_);

    dirty_ = true;
}

void PageSprite::rotateByOneStep()
{
    switch (dir_) {
    case PageDirection::Up:
        dir_ = PageDirection::Right;
        break;
    case PageDirection::Right:
        dir_ = PageDirection::Down;
        break;
    case PageDirection::Down:
        dir_ = PageDirection::Left;
        break;
    case PageDirection::Left:
        dir_ = PageDirection::Up;
        break;
    default:
        break;
    }

    rotatedSize_ = calcRotatedSize(rawImg_.size(), dir_);

    dirty_ = true;
}

void PageSprite::moveTo(const QPoint &pos)
{
    pos_ = pos;
    dirty_ = true;
}

void PageSprite::moveBy(int dx, int dy)
{
    pos_.rx() += dx;
    pos_.ry() += dy;
    dirty_ = true;
}

bool PageSprite::isMovable(const QSize &areaSize) const
{
    return std::floor(rotatedSize_.width() * scale_) > areaSize.width()
        || std::floor(rotatedSize_.height() * scale_) > areaSize.height();
}

void PageSprite::setScale(float f)
{
    scale_ = f;
    dirty_ = true;
}

void PageSprite::updateMatrix() const
{
    if (!dirty_) {
        return;
    }

    matrix_.reset();

    matrix_.translate(pos_.x(), pos_.y());
    matrix_.rotate(toDegree(dir_));
    matrix_.scale(scale_, scale_);
    

    dirty_ = false;
}

}
