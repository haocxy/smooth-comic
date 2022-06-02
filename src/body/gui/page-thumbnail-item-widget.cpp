#include "page-thumbnail-item-widget.h"

#include <QPainter>
#include <QBoxLayout>

namespace myapp {

PageThumbnailItemWidget::PageThumbnailItemWidget(const QPixmap &img, const QString &text, QWidget *parent)
    : QWidget(parent)
{
    img_ = new QLabel(this);
    img_->setPixmap(img);

    text_ = new QLabel(this);
    text_->setText(text);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(img_);
    layout->addWidget(text_);
}

void PageThumbnailItemWidget::paintEvent(QPaintEvent *)
{
    QSize size = this->size();

    QPainter p;
    p.fillRect(0, 0, size.width(), size.height(), Qt::blue);


}

}
