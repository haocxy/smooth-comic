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

void ThumbImg::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    if (!img_.isNull()) {
        p.drawPixmap(rect(), img_);
    } else {
        p.fillRect(rect(), Qt::lightGray);
    }
}

void ThumbImg::showEvent(QShowEvent *)
{
    book_.loadThumbImg(seqNum_, [this, h = handle_.weak()](const QPixmap &img) {
        h.apply([this, &img] {
            strandEntry_.exec([this, img] {
                img_ = img;
                update();
            });
        });
    });
}

void ThumbImg::moveEvent(QMoveEvent *e)
{
}

}
