#pragma once

#include <QObject>
#include <QPixmap>
#include <QPoint>
#include <QRect>
#include <QSize>
#include <QTransform>

#include "book/page-num.h"

#include "page-direction.h"


class QPainter;


namespace myapp {

class PageSprite : public QObject {
    Q_OBJECT
public:
    explicit PageSprite(PageNum seqNum, const QPixmap &rawImg, QObject *parent = nullptr);

    virtual ~PageSprite();

    PageNum pageSeq() const {
        return seqNum_;
    }

    void draw(QPainter &painter) const;

    qreal calcMinScale(const QSizeF &areaSize) const;

    qreal calcMaxScale(const QSizeF &areaSize) const;

    void adjustAreaSize(const QSizeF &areaSize);

    void adjustAreaWidth(qreal areaWidth);

    void adjustAreaHeight(qreal areaHeight);

    void rotateTo(PageDirection direction);

    qreal scale() const {
        return scale_;
    }

    void setScale(qreal f);

    void moveTo(const QPointF &pos);

    void moveBy(qreal dx, qreal dy);

    bool isMovable(const QSizeF &areaSize) const;

    QPointF pos() const {
        return pos_;
    }

    QSizeF realSize() const {
        return QSizeF(
            rotatedSize_.width() * scale_,
            rotatedSize_.height() * scale_
        );
    }

    QRectF spriteRect() const {
        return spriteRectForPos(pos_);
    }

    QRectF spriteRectForPos(const QPointF &pos) const {
        const QSizeF realSize = this->realSize();
        QRectF rect(QPointF(0, 0), realSize);
        rect.translate(-realSize.width() * anchor_.x(), -realSize.height() * anchor_.y());
        rect.translate(pos);
        return rect;
    }

private:
    void updateMatrix() const;

private:
    const PageNum seqNum_;

    QPixmap rawImg_;

    // 锚点在显示区域的绝对坐标
    QPointF pos_{ 0, 0 };

    // 锚点，范围为 0 到 1
    QPointF anchor_{ 0.5, 0.5 };

    PageDirection dir_{ PageDirection::Up };

    QSizeF rotatedSize_;

    qreal scale_{ 1 };

    mutable QTransform matrix_;

    mutable bool dirty_{ true };
};


}
