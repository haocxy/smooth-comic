#pragma once

#include <QWidget>


namespace myapp {

class ControllBarArea : public QWidget {
    Q_OBJECT
public:
    explicit ControllBarArea(QWidget *parent = nullptr);

    virtual ~ControllBarArea();
};

}
