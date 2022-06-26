#include "page-viewer.h"

#include <QPainter>

#include "thumb-list-scroll-area.h"
#include "page-switcher.h"
#include "page-controll-layer.h"

#include "core/logger.h"


namespace myapp {

using logger::gLogger;


PageViewer::PageViewer(Book &book, QWidget *parent)
    : QSplitter(parent)
    , book_(book)
{
    setChildrenCollapsible(false);

    thumbArea_ = new ThumbListScrollArea(book_, this);

    centerArea_ = new QWidget(this);

    centerLayout_ = new QStackedLayout(this);
    centerArea_->setLayout(centerLayout_);

    pageSwitcher_ = new PageSwitcher(book_, this);
    centerLayout_->addWidget(pageSwitcher_);

    pageControllLayer_ = new PageControllLayer(this);
    centerLayout_->addWidget(pageControllLayer_);

    addWidget(thumbArea_);
    addWidget(centerArea_);

    setSizes({ 1, 1000 });

    connect(thumbArea_->thumbList(), &ThumbList::sigJumpTo, this, &PageViewer::jumpTo);
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
