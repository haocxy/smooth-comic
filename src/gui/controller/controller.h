#pragma once

#include <QObject>


namespace myapp {

class Controller : public QObject {
    Q_OBJECT
public:
    explicit Controller(QObject *parent = nullptr);

    virtual ~Controller();
};

}
