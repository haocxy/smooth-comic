#pragma once

#include <QObject>
#include <QPixmap>


namespace myapp {

class PageSprite : public QObject {
    Q_OBJECT
public:
    explicit PageSprite(const QPixmap &rawImg, QObject *parent = nullptr);

    virtual ~PageSprite();

private:
    QPixmap rawImg_;
};


}
