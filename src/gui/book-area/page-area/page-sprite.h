#pragma once

#include <QObject>
#include <QPixmap>
#include <QPoint>
#include <QRect>
#include <QSize>
#include <QTransform>


class QPainter;


namespace myapp {

class PageSprite : public QObject {
    Q_OBJECT
public:
    explicit PageSprite(const QPixmap &rawImg, QObject *parent = nullptr);

    virtual ~PageSprite();

    void draw(QPainter &painter) const;



private:
    void updateMatrix();

private:
    QPixmap rawImg_;

    QPointF pos_{ 0, 0 };

    QPointF anchor_{ 0.5, 0.5 };

    int rotateDegree_{ 0 };

    float scale_{ 1 };

    QTransform matrix_;
};


}
