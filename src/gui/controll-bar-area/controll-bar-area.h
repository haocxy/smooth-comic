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
    QPointer<QHBoxLayout> rootLayout_;

    // left
    QPointer<QWidget> leftArea_;
    QPointer<QHBoxLayout> leftLayout_;
    QPointer<ControllItem> ctrToggleThumbArea_;

    // center
    QPointer<QWidget> centerArea_;
    QPointer<QHBoxLayout> centerLayout_;
    QPointer<ControllItem> ctrOpen_;
    QPointer<ControllItem> ctrReload_;
    QPointer<ControllItem> ctrSwitchLeft_;
    QPointer<ControllItem> ctrSwitchRight_;

    // right
    QPointer<QWidget> rightArea_;
    QPointer<QHBoxLayout> rightLayout_;
    QPointer<ControllItem> ctrGlobalMenu_;
};

}
