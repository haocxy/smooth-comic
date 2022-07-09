#include "controll-bar-area.h"


#include "controller/controller.h"

#include "controll-item.h"


namespace myapp {

ControllBarArea::ControllBarArea(Controller &controller, QWidget *parent)
    : QFrame(parent)
    , controller_(controller)
{
    setObjectName("ControllBarArea");

    rootLayout_ = new QHBoxLayout(this);
    rootLayout_->setContentsMargins(QMargins());
    setLayout(rootLayout_);

    leftArea_ = new QWidget(this);
    leftLayout_ = new QHBoxLayout(leftArea_);
    leftArea_->setLayout(leftLayout_);
    rootLayout_->addWidget(leftArea_);

    rootLayout_->addStretch();

    centerArea_ = new QWidget(this);
    centerLayout_ = new QHBoxLayout(centerArea_);
    centerArea_->setLayout(centerLayout_);
    rootLayout_->addWidget(centerArea_);

    rootLayout_->addStretch();

    rightArea_ = new QWidget(this);
    rightLayout_ = new QHBoxLayout(rightArea_);
    rightArea_->setLayout(rightLayout_);
    rootLayout_->addWidget(rightArea_);

    ctrToggleThumbArea_ = new ControllItem(FontIconEnum::DockRightMirrored, tr("Thumb Area"), this);
    leftLayout_->addWidget(ctrToggleThumbArea_);
    connect(ctrToggleThumbArea_, &ControllItem::clicked, &controller_, &Controller::cmdToggleThumbArea);

    ctrOpen_ = new ControllItem(FontIconEnum::OpenFile, tr("Open"), this);
    centerLayout_->addWidget(ctrOpen_);
    connect(ctrOpen_, &ControllItem::clicked, &controller_, &Controller::cmdOpenComic);

    ctrReload_ = new ControllItem(FontIconEnum::Refresh, tr("Reload"), this);
    centerLayout_->addWidget(ctrReload_);
    connect(ctrReload_, &ControllItem::clicked, &controller_, &Controller::reloadComic);

    ctrSwitchLeft_ = new ControllItem(FontIconEnum::SwitchLeft, tr("Left"), this);
    centerLayout_->addWidget(ctrSwitchLeft_);
    connect(ctrSwitchLeft_, &ControllItem::clicked, &controller_,
        std::bind(&Controller::cmdSwitchPage, &controller_, SwitchDirection::Left));

    ctrSwitchRight_ = new ControllItem(FontIconEnum::SwitchRight, tr("Right"), this);
    centerLayout_->addWidget(ctrSwitchRight_);
    connect(ctrSwitchRight_, &ControllItem::clicked, &controller_,
        std::bind(&Controller::cmdSwitchPage, &controller_, SwitchDirection::Right));


    ctrGlobalMenu_ = new ControllItem(FontIconEnum::More, tr("Menu"), this);
    rightLayout_->addWidget(ctrGlobalMenu_);
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
