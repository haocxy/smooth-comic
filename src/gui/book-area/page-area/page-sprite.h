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

    void move(const QPointF &pos);

    void rotate(PageDirection direction);



private:
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
