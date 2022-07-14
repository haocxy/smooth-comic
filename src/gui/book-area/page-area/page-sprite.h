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

    float calcMinScale(const QSizeF &areaSize) const;

    float calcMaxScale(const QSizeF &areaSize) const;

    void adjustAreaSize(const QSizeF &areaSize);

    void adjustAreaWidth(int areaWidth);

    void adjustAreaHeight(int areaHeight);

    void rotateTo(PageDirection direction);

    void rotateByOneStep();

    void moveTo(const QPointF &pos);

    void moveBy(qreal dx, qreal dy);

    bool isMovable(const QSizeF &areaSize) const;

    QSizeF rotatedSize() const;

private:
    void scale(float f);

    void updateMatrix() const;

private:
    const PageNum seqNum_;

    QPixmap rawImg_;

    QPointF pos_{ 0, 0 };

    QPointF anchor_{ 0.5, 0.5 };

    PageDirection dir_{ PageDirection::Up };

    QSizeF rotatedSize_;

    float scale_{ 1 };

    mutable QTransform matrix_;

    mutable bool dirty_{ true };
};




}
