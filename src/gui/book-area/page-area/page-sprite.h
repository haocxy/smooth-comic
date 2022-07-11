#pragma once

#include <QObject>
#include <QPixmap>
#include <QPoint>
#include <QRect>
#include <QSize>


class QPainter;


namespace myapp {

class PageSprite : public QObject {
    Q_OBJECT
public:
    explicit PageSprite(const QPixmap &rawImg, QObject *parent = nullptr);

    virtual ~PageSprite();

    void draw(QPainter &painter) const;

private:
    QPixmap rawImg_;

    QPointF anchor_{ 0.5, 0.5 };

    QSize showSize_;
};


}
