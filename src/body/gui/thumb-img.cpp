#include "thumb-img.h"

#include <QPainter>
#include <QMoveEvent>

#include "core/logger.h"

#include "book/book.h"


namespace myapp {

using logger::gLogger;

ThumbImg::ThumbImg(Book &book, PageNum seqNum, i32 width, i32 height, QWidget *parent)
    : QWidget(parent)
    , book_(book)
    , seqNum_(seqNum)
    , width_(width)
    , height_(height)
    , handle_(*this)
{
    QSizePolicy spImg(sizePolicy());
    spImg.setVerticalPolicy(QSizePolicy::Policy::Ignored);
    spImg.setHorizontalPolicy(QSizePolicy::Policy::Expanding);
    spImg.setHeightForWidth(true);
    setSizePolicy(spImg);
}

void ThumbImg::updateThumbVisiableState(bool isVisiable)
{
    opt<bool> oldIsVisiable = isVisiable_;
    isVisiable_ = isVisiable;
    
    if (!oldIsVisiable.has_value() || *oldIsVisiable != isVisiable) {
        onThumbVisiableStateSwithed(oldIsVisiable, isVisiable);
    }
}

void ThumbImg::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    if (!img_.isNull()) {
        p.drawPixmap(rect(), img_);
    } else {
        p.fillRect(rect(), Qt::lightGray);
    }
}

void ThumbImg::onThumbVisiableStateSwithed(opt<bool> oldIsVisiable, bool newIsVisiable)
{
    if (newIsVisiable) {
        loadImg();
    } else {
        releaseImg();
    }
}

void ThumbImg::loadImg()
{
    if (isImgLoading_) {
        return;
    }

    if (!img_.isNull()) {
        return;
    }

    book_.loadThumbImg(seqNum_, [this, h = handle_.weak()](const QPixmap &img) {
        h.apply([this, &img] {
            strandEntry_.exec([this, img] {
                img_ = img;
                isImgLoading_ = false;
                update();
            });
        });
    });

    isImgLoading_ = true;
}

void ThumbImg::releaseImg()
{
    if (img_.isNull()) {
        return;
    }

    img_ = QPixmap();

    gLogger.d << "ThumbImg(" << seqNum_ << ") releaseImg()";
}

}
