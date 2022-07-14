#include "page-area.h"


#include "page-director.h"


namespace myapp {

PageArea::PageArea(Controller &controller, QWidget *parent)
    : QWidget(parent)
{
    director_ = new PageDirector(controller);

    layout_ = new QStackedLayout(this);
    layout_->setStackingMode(QStackedLayout::StackingMode::StackAll);
    setLayout(layout_);

    controllLayer_ = new PageControllLayer(controller, *director_, this);
    layout_->addWidget(controllLayer_);

    switcher_ = new PageSwitcher(controller, *director_, this);
    layout_->addWidget(switcher_);
}

PageArea::~PageArea()
{

}

}
