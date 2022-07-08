#pragma once

#include <QWidget>
#include <QPointer>
#include <QBoxLayout>


namespace myapp {

class Controller;

class ControllItem;


class ControllBarArea : public QWidget {
    Q_OBJECT
public:
    explicit ControllBarArea(Controller &controller, QWidget *parent = nullptr);

    virtual ~ControllBarArea();

private:
    Controller &controller_;
    QPointer<QHBoxLayout> layout_;
    QPointer<ControllItem> ctrOpen_;
    QPointer<ControllItem> ctrReload_;
};

}
