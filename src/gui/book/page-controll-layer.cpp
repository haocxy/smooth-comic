#include "page-controll-layer.h"

#include <QPainter>
#include <QMouseEvent>


namespace myapp {

PageControllLayer::PageControllLayer(QWidget *parent)
    : QWidget(parent)
{
    QSizePolicy sp{ sizePolicy() };
    sp.setHorizontalStretch(QSizePolicy::Policy::Expanding);
    sp.setVerticalPolicy(QSizePolicy::Policy::Expanding);
    setSizePolicy(sp);
}

PageControllLayer::~PageControllLayer()
{
}

void PageControllLayer::resizeEvent(QResizeEvent *e)
{
    QWidget::resizeEvent(e);

    areaConfig_.setTotalArea(size());
}

void PageControllLayer::paintEvent(QPaintEvent *e)
{
    QWidget::paintEvent(e);

    QPainter p(this);

    if (showControllAreas_) {
        const int w = width();
        const int h = height();

        QColor color(Qt::gray);
        color.setAlphaF(0.5);
        p.fillRect(areaConfig_.leftSwitcher(), color);
        p.fillRect(areaConfig_.rightSwitcher(), color);
    }
}

void PageControllLayer::mouseReleaseEvent(QMouseEvent *e)
{
    if (areaConfig_.leftSwitcher().contains(e->pos())) {
        emit sigCmdSwitchNextPage(SwitchDirection::Left);
        return;
    }

    if (areaConfig_.rightSwitcher().contains(e->pos())) {
        emit sigCmdSwitchNextPage(SwitchDirection::Right);
        return;
    }
}

}
