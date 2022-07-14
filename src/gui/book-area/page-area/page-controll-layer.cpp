#include "page-controll-layer.h"

#include <QPainter>
#include <QMouseEvent>

#include "controller/controller.h"

#include "page-director.h"


namespace myapp {

PageControllLayer::PageControllLayer(Controller &controller, PageDirector &director, QWidget *parent)
    : QWidget(parent)
    , controller_(controller)
    , director_(director)
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

void PageControllLayer::enterEvent(QEnterEvent *e)
{
    isPageMovable_ = director_.isPageMovable();

    if (isPageMovable_) {
        setCursor(Qt::CursorShape::OpenHandCursor);
    } else {
        setCursor(Qt::CursorShape::ArrowCursor);
    }
}

void PageControllLayer::mousePressEvent(QMouseEvent *e)
{
    if (isPageMovable_) {
        isPageMoving_ = true;
        setCursor(Qt::CursorShape::ClosedHandCursor);

        moveStart_ = e->pos();
    }
}

void PageControllLayer::mouseReleaseEvent(QMouseEvent *e)
{
    if (isPageMoving_) {

        isPageMoving_ = false;
        setCursor(Qt::CursorShape::OpenHandCursor);

    } else {

        if (areaConfig_.leftSwitcher().contains(e->pos())) {
            emit controller_.cmdSwitchPage(SwitchDirection::Left);
        } else if (areaConfig_.rightSwitcher().contains(e->pos())) {
            emit controller_.cmdSwitchPage(SwitchDirection::Right);
        }

    }
}

void PageControllLayer::mouseMoveEvent(QMouseEvent *e)
{
    if (isPageMoving_) {
        const QPoint moveStart = moveStart_;
        const QPoint moveEnd = e->pos();
        moveStart_ = moveEnd;

        const QPoint moveDelta = moveEnd - moveStart;
        director_.movePage(moveDelta.x(), moveDelta.y());
    }
}

}
