#include "thumb-list-scroll-area.h"

#include "book/book.h"
#include "thumb-item.h"
#include "thumb-list.h"


namespace myapp {

static constexpr int widthHint = 200;

ThumbListScrollArea::ThumbListScrollArea(Book &book, QWidget *parent)
    : QScrollArea(parent)
    , book_(book)
{
    setHorizontalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOn);

    setMinimumWidth(widthHint);
    setMaximumWidth(widthHint * 2);

    jumpBar_ = new JumpBar(this);
    setVerticalScrollBar(jumpBar_);

    root_ = new ThumbList(book, this);
    setWidget(root_);

    setWidgetResizable(true);
}

void ThumbListScrollArea::resizeEvent(QResizeEvent *)
{
    root_->adjustSize();
}


}
