#pragma once

#include <QObject>
#include <QPixmap>
#include <QPoint>
#include <QRect>
#include <QSize>
#include <QTransform>

#include "page-direction.h"


class QPainter;


namespace myapp {

class PageSprite : public QObject {
    Q_OBJECT
public:
    explicit PageSprite(const QPixmap &rawImg, QObject *parent = nullptr);

    virtual ~PageSprite();

    void draw(QPainter &painter) const;

    void adjustArea(const QSizeF &areaSize);

    void rotateTo(PageDirection direction);

    void rotateByOneStep();

    void moveTo(const QPointF &pos);

    void moveBy(qreal dx, qreal dy);

private:
    void scale(float f);

    void updateMatrix() const;

private:
    QPixmap rawImg_;

    QPointF pos_{ 0, 0 };

    QPointF anchor_{ 0.5, 0.5 };

    PageDirection dir_{ PageDirection::Up };

    float scale_{ 1 };

    mutable QTransform matrix_;

    mutable bool dirty_{ true };
};




}
