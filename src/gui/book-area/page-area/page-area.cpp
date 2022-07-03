#include "page-area.h"


namespace myapp {

PageArea::PageArea(Book &book, QWidget *parent)
    : QWidget(parent)
{
    layout_ = new QStackedLayout(this);
    layout_->setStackingMode(QStackedLayout::StackingMode::StackAll);
    setLayout(layout_);

    controllLayer_ = new PageControllLayer(this);
    layout_->addWidget(controllLayer_);

    switcher_ = new PageSwitcher(book, this);
    layout_->addWidget(switcher_);

    connect(controllLayer_, &PageControllLayer::sigCmdSwitchNextPage, switcher_, &PageSwitcher::switchNextPage);
}

PageArea::~PageArea()
{

}

void PageArea::jumpTo(PageNum pageNum)
{
    switcher_->jumpTo(pageNum);
}

void PageArea::jumpNext()
{
    switcher_->jumpNext();
}

void PageArea::jumpPrev()
{
    switcher_->jumpPrev();
}

}
