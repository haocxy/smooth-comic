#include "book-area.h"

#include <QPainter>

#include "core/logger.h"

#include "page-switcher.h"
#include "page-controll-layer.h"
#include "book-status.h"

#include "thumb/thumb-list-scroll-area.h"


namespace myapp {

using logger::gLogger;


PageViewer::PageViewer(Book &book, QWidget *parent)
    : QSplitter(parent)
    , book_(book)
{
    setChildrenCollapsible(false);

    thumbArea_ = new ThumbListScrollArea(book_, this);

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

    connect(thumbArea_->thumbList(), &ThumbList::sigJumpTo, this, &PageViewer::jumpTo);

    connect(pageControllLayer_, &PageControllLayer::sigCmdSwitchNextPage, pageSwitcher_, &PageSwitcher::switchNextPage);
}

void PageViewer::jumpTo(PageNum seqNum)
{
    pageSwitcher_->jumpTo(seqNum);
}

void PageViewer::jumpNext()
{
    pageSwitcher_->jumpNext();
}

void PageViewer::jumpPrev()
{
    pageSwitcher_->jumpPrev();
}

}
