#include "page-area.h"


namespace myapp {

PageArea::PageArea(Controller &controller, QWidget *parent)
    : QWidget(parent)
{
    layout_ = new QStackedLayout(this);
    layout_->setStackingMode(QStackedLayout::StackingMode::StackAll);
    setLayout(layout_);

    controllLayer_ = new PageControllLayer(controller, this);
    layout_->addWidget(controllLayer_);

    switcher_ = new PageSwitcher(controller, this);
    layout_->addWidget(switcher_);
}

PageArea::~PageArea()
{

}

}
