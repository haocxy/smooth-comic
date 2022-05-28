#include "artboard-widget.h"

#include <QPainter>


namespace myapp {

ArtboardWidget::ArtboardWidget(QWidget *parent)
    : QWidget(parent)
{
}

void ArtboardWidget::paintEvent(QPaintEvent *)
{
    const QSize s = this->size();

    QPainter p(this);
    p.drawLine(100, 100, s.width() - 100, s.height() - 100);
}

}
