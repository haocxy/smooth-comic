#include "controll-bar-area.h"

#include "controll-item.h"


namespace myapp {

ControllBarArea::ControllBarArea(Controller &controller, QWidget *parent)
    : QWidget(parent)
    , controller_(controller)
{
    layout_ = new QHBoxLayout(this);
    setLayout(layout_);

    layout_->addWidget(new ControllItem(FontIconEnum::ChromeBackToWindow, "A1", this));
    layout_->addWidget(new ControllItem(FontIconEnum::ChromeFullScreen, "A2", this));
}

ControllBarArea::~ControllBarArea()
{
}

}
