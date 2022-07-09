#include "controll-bar-area.h"


#include "controller/controller.h"

#include "controll-item.h"


namespace myapp {

ControllBarArea::ControllBarArea(Controller &controller, QWidget *parent)
    : QFrame(parent)
    , controller_(controller)
{
    setObjectName("ControllBarArea");

    layout_ = new QHBoxLayout(this);
    setLayout(layout_);

    ctrToggleThumbArea_ = new ControllItem(FontIconEnum::DockRightMirrored, tr("Thumb Area"), this);
    layout_->addWidget(ctrToggleThumbArea_);
    connect(ctrToggleThumbArea_, &ControllItem::clicked, &controller_, &Controller::cmdToggleThumbArea);

    layout_->addStretch();

    ctrOpen_ = new ControllItem(FontIconEnum::OpenFile, tr("Open"), this);
    layout_->addWidget(ctrOpen_);
    connect(ctrOpen_, &ControllItem::clicked, &controller_, &Controller::cmdOpenComic);

    ctrReload_ = new ControllItem(FontIconEnum::Refresh, tr("Reload"), this);
    layout_->addWidget(ctrReload_);
    connect(ctrReload_, &ControllItem::clicked, &controller_, &Controller::reloadComic);

    ctrSwitchLeft_ = new ControllItem(FontIconEnum::SwitchLeft, tr("Left"), this);
    layout_->addWidget(ctrSwitchLeft_);
    connect(ctrSwitchLeft_, &ControllItem::clicked, &controller_,
        std::bind(&Controller::cmdSwitchPage, &controller_, SwitchDirection::Left));

    ctrSwitchRight_ = new ControllItem(FontIconEnum::SwitchRight, tr("Right"), this);
    layout_->addWidget(ctrSwitchRight_);
    connect(ctrSwitchRight_, &ControllItem::clicked, &controller_,
        std::bind(&Controller::cmdSwitchPage, &controller_, SwitchDirection::Right));

    layout_->addStretch();

    ctrGlobalMenu_ = new ControllItem(FontIconEnum::More, tr("Menu"), this);
    layout_->addWidget(ctrGlobalMenu_);
    // TODO connect
}

ControllBarArea::~ControllBarArea()
{
}

bool ControllBarArea::isWindowMoveAreaContainsLocalPos(const QPoint &localPos) const
{
    return childAt(localPos) == nullptr;
}

}
