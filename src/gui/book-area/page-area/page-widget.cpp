#include "page-widget.h"

#include <QPainter>
#include <QStyle>


namespace myapp {

PageWidget::PageWidget(Book &book, PageNum seqNum, const QPixmap &img, QWidget *parent)
    : QWidget(parent)
    , book_(book)
    , seqNum_(seqNum)
    , img_(img)
{
    setObjectName("PageWidget");
}

PageWidget::~PageWidget()
{
}

static QSize calcShowSize(const QSize &imgSize, const QSize &areaSize)
{
    if (imgSize.width() > 0 && imgSize.height() > 0) {
        const float scaleW = float(areaSize.width()) / imgSize.width();
        const float scaleH = float(areaSize.height()) / imgSize.height();
        const float scale = std::min(scaleW, scaleH);
        return imgSize * scale;
    } else {
        return QSize();
    }
}

void PageWidget::paintEvent(QPaintEvent *e)
{
    QPainter p(this);
    p.fillRect(rect(), palette().color(QPalette::Base));

    const QSize areaSize = size();
    const QSize showSize = calcShowSize(img_.size(), areaSize);

    if (showSize.isValid()) {
        const auto x = (areaSize.width() - showSize.width()) / 2;
        const auto y = (areaSize.height() - showSize.height()) / 2;
        const QRect showRect = QRect(QPoint(x, y), showSize);
        p.drawPixmap(showRect, img_);
    }
}

}
