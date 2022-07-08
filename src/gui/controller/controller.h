#pragma once

#include <QObject>

#include "switch-direction.h"


namespace myapp {

class Book;

class Controller : public QObject {
    Q_OBJECT
public:
    explicit Controller(Book &book, QObject *parent = nullptr);

    virtual ~Controller();

    Book &book() {
        return book_;
    }

signals:
    void cmdSwitchPage(SwitchDirection direction);

private:
    Book &book_;
};

}
