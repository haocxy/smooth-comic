#pragma once

#include <QFrame>
#include <QPointer>
#include <QBoxLayout>

#include "controll-item.h"


namespace myapp {

class Controller;

class PopupLayer;

class ScaleSettingPopup;


class ControllBarArea : public QFrame {
    Q_OBJECT
public:
    explicit ControllBarArea(Controller &controller, PopupLayer &popupLayer, QWidget *parent = nullptr);

    virtual ~ControllBarArea();

    bool isWindowMoveAreaContainsLocalPos(const QPoint &localPos) const;

private:
    void toggleScaleSetting();

    void bind(ControllItem *controllItem, void(Controller:: *memFun)());

    template <typename Arg>
    void bind(ControllItem *controllItem, void(Controller:: *memFun)(Arg), Arg arg) {
        connect(controllItem, &ControllItem::clicked, &controller_,
            std::bind(memFun, &controller_, arg));
    }

    void bind(ControllItem *controllItem, void(ControllBarArea:: *memFun)()) {
        connect(controllItem, &ControllItem::clicked, this, memFun);
    }

private:
    Controller &controller_;
    PopupLayer &popupLayer_;

private:
    QPointer<QHBoxLayout> rootLayout_;

    // left
    QPointer<QWidget> leftArea_;
    QPointer<QHBoxLayout> leftLayout_;
    QPointer<ControllItem> ctrToggleThumbArea_;

    // center
    QPointer<QWidget> centerArea_;
    QPointer<QHBoxLayout> centerLayout_;
    QPointer<ControllItem> ctrOpen_;
    QPointer<ControllItem> ctrRotate_;
    QPointer<ControllItem> ctrSwitchLeft_;
    QPointer<ControllItem> ctrSwitchRight_;
    QPointer<ControllItem> ctrScale_;

    // right
    QPointer<QWidget> rightArea_;
    QPointer<QHBoxLayout> rightLayout_;
    QPointer<ControllItem> ctrGlobalMenu_;

    // menu
    QPointer<ScaleSettingPopup> scaleSettingPopup_;
};

}
