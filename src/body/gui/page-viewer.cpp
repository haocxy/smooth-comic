#include "page-viewer.h"

#include <QPainter>


namespace myapp {

PageViewerWidget::PageViewerWidget(Book &book, QWidget *parent)
    : QWidget(parent)
    , book_(book)
{
}

void PageViewerWidget::paintEvent(QPaintEvent *)
{
    const QSize s = this->size();

    QPainter p(this);
    p.drawLine(100, 100, s.width() - 100, s.height() - 100);
}

}
