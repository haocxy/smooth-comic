#include "page-viewer.h"

#include <QPainter>


namespace myapp {

PageViewerWidget::PageViewerWidget(QWidget *parent)
    : QWidget(parent)
{
}

void PageViewerWidget::paintEvent(QPaintEvent *)
{
    const QSize s = this->size();

    QPainter p(this);
    p.drawLine(100, 100, s.width() - 100, s.height() - 100);
}

}
