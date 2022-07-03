#include "book-area.h"

#include <QPainter>

#include "core/logger.h"

#include "status-area/book-status.h"

#include "page-area/page-area.h"


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

    pageArea_ = new PageArea(book, this);
    mainAreaLayout_->addWidget(pageArea_);

    bookStatus_ = new BookStatus(book_, this);
    mainAreaLayout_->addWidget(bookStatus_);

    addWidget(thumbArea_);
    addWidget(mainArea_);

    setSizes({ 1, 1000 });

    connect(thumbArea_->thumbList(), &ThumbList::sigJumpTo, this, &BookArea::jumpTo);
}

void BookArea::jumpTo(PageNum seqNum)
{
    pageArea_->jumpTo(seqNum);
}

void BookArea::jumpNext()
{
    pageArea_->jumpNext();
}

void BookArea::jumpPrev()
{
    pageArea_->jumpPrev();
}

}
