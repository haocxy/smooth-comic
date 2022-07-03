#include "book-area.h"

#include <QPainter>

#include "core/logger.h"

#include "page-switcher.h"
#include "page-controll-layer.h"
#include "book-status.h"


namespace myapp {

using logger::gLogger;


BookArea::BookArea(Book &book, QWidget *parent)
    : QSplitter(parent)
    , book_(book)
{
    setChildrenCollapsible(false);

    thumbArea_ = new ThumbArea(book_, this);

    mainArea_ = new QWidget(this);

    mainAreaLayout_ = new QVBoxLayout(this);
    mainArea_->setLayout(mainAreaLayout_);

    pageArea_ = new QWidget(this);
    

    pageAreaLayout_ = new QStackedLayout(this);
    pageAreaLayout_->setStackingMode(QStackedLayout::StackingMode::StackAll);
    pageArea_->setLayout(pageAreaLayout_);

    mainAreaLayout_->addWidget(pageArea_);

    pageSwitcher_ = new PageSwitcher(book_, this);
    pageControllLayer_ = new PageControllLayer(this);

    bookStatus_ = new BookStatus(book_, this);
    mainAreaLayout_->addWidget(bookStatus_);

    pageAreaLayout_->addWidget(pageControllLayer_);
    pageAreaLayout_->addWidget(pageSwitcher_);

    addWidget(thumbArea_);
    addWidget(mainArea_);

    setSizes({ 1, 1000 });

    connect(thumbArea_->thumbList(), &ThumbList::sigJumpTo, this, &BookArea::jumpTo);

    connect(pageControllLayer_, &PageControllLayer::sigCmdSwitchNextPage, pageSwitcher_, &PageSwitcher::switchNextPage);
}

void BookArea::jumpTo(PageNum seqNum)
{
    pageSwitcher_->jumpTo(seqNum);
}

void BookArea::jumpNext()
{
    pageSwitcher_->jumpNext();
}

void BookArea::jumpPrev()
{
    pageSwitcher_->jumpPrev();
}

}
