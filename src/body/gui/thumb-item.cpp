#include "thumb-item.h"

#include <QPainter>
#include <QBoxLayout>

namespace myapp {

ThumbItem::ThumbItem(PageNum pageNum, const QPixmap &img, const QString &text, QWidget *parent)
    : QWidget(parent)
    , pageNum_(pageNum)
{
    img_ = new ThumbImgWidget(pageNum_, img, this);
    QSizePolicy spImg = img_->sizePolicy();
    spImg.setVerticalPolicy(QSizePolicy::Policy::Ignored);
    spImg.setHorizontalPolicy(QSizePolicy::Policy::Expanding);
    img_->setSizePolicy(spImg);

    text_ = new QLabel(this);
    text_->setText(text);
    text_->setScaledContents(true);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(img_);
    layout->addWidget(text_);

    setLayout(layout);

    //setMaximumWidth(100);
    setMinimumHeight(100);
}

}
