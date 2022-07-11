#pragma once

#include <QObject>


namespace myapp {

class PageSprite : public QObject {
    Q_OBJECT
public:
    explicit PageSprite(QObject *parent = nullptr);

    virtual ~PageSprite();
};


}
