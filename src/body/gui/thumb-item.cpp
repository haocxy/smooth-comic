#include "thumb-item.h"

#include <QPainter>
#include <QBoxLayout>

namespace myapp {

ThumbItem::ThumbItem(Book &book, PageNum seqNum, const u32str &entryPath, i32 width, i32 height, QWidget *parent)
    : QWidget(parent)
    , entryPath_(entryPath)
{
    img_ = new ThumbImg(book, seqNum, width, height, this);

    text_ = new QLabel(this);
    text_->setText(QString::fromStdU32String(entryPath_) + "(index:" + QString::number(seqNum) + ")");
    text_->setScaledContents(true);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(img_);
    layout->addWidget(text_);

    setLayout(layout);

    //setMaximumWidth(100);
    setMinimumHeight(100);
}

void ThumbItem::updateThumbVisiableState()
{
    img_->updateThumbVisiableState(!visibleRegion().isEmpty());
}

void ThumbItem::moveEvent(QMoveEvent *e)
{
    QWidget::moveEvent(e);

    updateThumbVisiableState();
}

}
