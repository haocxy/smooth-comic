#include "page-area.h"


#include "page-scene.h"


namespace myapp {

PageArea::PageArea(Controller &controller, QWidget *parent)
    : QWidget(parent)
{
    scene_ = new PageScene(controller);

    layout_ = new QStackedLayout(this);
    layout_->setStackingMode(QStackedLayout::StackingMode::StackAll);
    setLayout(layout_);

    controllLayer_ = new PageControllLayer(controller, *scene_, this);
    layout_->addWidget(controllLayer_);

    switcher_ = new PageSwitcher(controller, *scene_, this);
    layout_->addWidget(switcher_);
}

PageArea::~PageArea()
{

}

}
