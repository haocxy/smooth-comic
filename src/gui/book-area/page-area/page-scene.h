#pragma once

#include <QObject>


namespace myapp {

class PageScene : public QObject {
    Q_OBJECT
public:
    explicit PageScene(QObject *parent = nullptr);

    virtual ~PageScene();
};

}
