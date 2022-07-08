#include "thumb-area.h"

#include "book/book.h"

#include "controller/controller.h"

#include "thumb-item.h"
#include "thumb-list.h"


namespace myapp {

static constexpr int widthHint = 200;

ThumbArea::ThumbArea(Controller &controller, QWidget *parent)
    : QScrollArea(parent)
    , controller_(controller)
{
    setObjectName("ThumbArea");

    setHorizontalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOn);

    setMinimumWidth(widthHint);
    setMaximumWidth(widthHint * 2);

    jumpBar_ = new JumpBar(this);
    setVerticalScrollBar(jumpBar_);

    connect(jumpBar_, &JumpBar::valueChanged, this, [this](int value) {
        scrollPercent_ = double(value) / jumpBar_->maximum();
    });

    root_ = new ThumbList(controller_, this);
    setWidget(root_);

    setWidgetResizable(true);
}

void ThumbArea::resizeEvent(QResizeEvent *)
{
    root_->adjustSize();

    if (scrollPercent_) {
        jumpBar_->setValue(static_cast<int>(jumpBar_->maximum() * scrollPercent_.value()));
    }
}


}
