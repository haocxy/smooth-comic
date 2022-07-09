#pragma once

#include <QFrame>
#include <QPointer>
#include <QBoxLayout>


namespace myapp {

class Controller;

class ControllItem;


class ControllBarArea : public QFrame {
    Q_OBJECT
public:
    explicit ControllBarArea(Controller &controller, QWidget *parent = nullptr);

    virtual ~ControllBarArea();

    bool isWindowMoveAreaContainsLocalPos(const QPoint &localPos) const;

private:
    Controller &controller_;
    QPointer<QHBoxLayout> layout_;

    // left
    QPointer<ControllItem> ctrToggleThumbArea_;

    // center
    QPointer<ControllItem> ctrOpen_;
    QPointer<ControllItem> ctrReload_;
    QPointer<ControllItem> ctrSwitchLeft_;
    QPointer<ControllItem> ctrSwitchRight_;

    // right
    QPointer<ControllItem> ctrGlobalMenu_;
};

}
