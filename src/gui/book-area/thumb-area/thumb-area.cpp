#include "thumb-area.h"

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

    connect(jumpBar_, &JumpBar::valueChanged, this, [this](int value) {
        scrollPercent_ = double(value) / jumpBar_->maximum();
    });

    root_ = new ThumbList(book, this);
    setWidget(root_);

    setWidgetResizable(true);
}

void ThumbListScrollArea::resizeEvent(QResizeEvent *)
{
    root_->adjustSize();

    if (scrollPercent_) {
        jumpBar_->setValue(static_cast<int>(jumpBar_->maximum() * scrollPercent_.value()));
    }
}


}
