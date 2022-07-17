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

    float calcMinScale(const QSize &areaSize) const;

    float calcMaxScale(const QSize &areaSize) const;

    void adjustAreaSize(const QSize &areaSize);

    void adjustAreaWidth(int areaWidth);

    void adjustAreaHeight(int areaHeight);

    void rotateTo(PageDirection direction);

    void rotateByOneStep();

    void setScale(float f);

    void moveTo(const QPoint &pos);

    void moveBy(int dx, int dy);

    bool isMovable(const QSize &areaSize) const;

    QPoint pos() const {
        return pos_;
    }

    QSize realSize() const {
        return QSize(
            rotatedSize_.width() * scale_,
            rotatedSize_.height() * scale_
        );
    }

    QRect spriteRect() const {
        return spriteRectForPos(pos_);
    }

    QRect spriteRectForPos(const QPoint &pos) const {
        const QSize realSize = this->realSize();
        QRect rect(QPoint(0, 0), realSize);
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
    QPoint pos_{ 0, 0 };

    // 锚点，范围为 0 到 1
    QPointF anchor_{ 0.5, 0.5 };

    PageDirection dir_{ PageDirection::Up };

    QSize rotatedSize_;

    qreal scale_{ 1 };

    mutable QTransform matrix_;

    mutable bool dirty_{ true };
};


}
