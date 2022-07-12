#include "popup-layer.h"

#include <QPainter>


namespace myapp {

PopupLayer::PopupLayer(QWidget *parent)
    : QWidget(parent)
{
    QSizePolicy sp{ sizePolicy() };
    sp.setHorizontalPolicy(QSizePolicy::Policy::Ignored);
    sp.setVerticalPolicy(QSizePolicy::Policy::Ignored);
    setSizePolicy(sp);
}

PopupLayer::~PopupLayer()
{
}

void PopupLayer::paintEvent(QPaintEvent *e)
{
    QWidget::paintEvent(e);

    QPainter p(this);
    p.drawRect(10, 10, width() - 20, height() - 20);
}

}
