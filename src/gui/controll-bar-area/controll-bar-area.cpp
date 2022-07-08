#include "controll-bar-area.h"


#include "controller/controller.h"

#include "controll-item.h"


namespace myapp {

ControllBarArea::ControllBarArea(Controller &controller, QWidget *parent)
    : QWidget(parent)
    , controller_(controller)
{
    layout_ = new QHBoxLayout(this);
    setLayout(layout_);

    layout_->addStretch();

    ctrOpen_ = new ControllItem(FontIconEnum::OpenFile, tr("Open"), this);
    layout_->addWidget(ctrOpen_);
    connect(ctrOpen_, &ControllItem::clicked, this, [this] {
        emit controller_.cmdOpenComic();
    });

    ctrReload_ = new ControllItem(FontIconEnum::Refresh, tr("Reload"), this);
    layout_->addWidget(ctrReload_);

    layout_->addStretch();
}

ControllBarArea::~ControllBarArea()
{
}

}
