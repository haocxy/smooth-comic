#include "page-controll-layer.h"

#include <QPainter>
#include <QMouseEvent>

#include "controller/controller.h"


namespace myapp {

PageControllLayer::PageControllLayer(Controller &controller, QWidget *parent)
    : QWidget(parent)
    , controller_(controller)
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
        emit controller_.cmdSwitchPage(SwitchDirection::Left);
        return;
    }

    if (areaConfig_.rightSwitcher().contains(e->pos())) {
        emit controller_.cmdSwitchPage(SwitchDirection::Right);
        return;
    }
}

}
