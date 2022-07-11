#include "popup.h"

#include <QBitmap>
#include <QPainter>


namespace myapp
{

PopupWidget::PopupWidget(QWidget *parent)
    : QFrame(parent) {

    setWindowFlag(Qt::Popup);
    setWindowFlag(Qt::FramelessWindowHint);
    setWindowFlag(Qt::NoDropShadowWindowHint);

    setAttribute(Qt::WA_TranslucentBackground);

    rootLayout_ = new QVBoxLayout(this);
    rootLayout_->setContentsMargins(QMargins());
    setLayout(rootLayout_);
}

PopupWidget::~PopupWidget()
{
}

void PopupWidget::locate(QWidget *base)
{
    if (!base) {
        return;
    }

    const QRect baseGeo = base->geometry();
    const QPoint basePos = base->mapToGlobal(QPoint(0, 0));
    const int baseMidX = basePos.x() + baseGeo.width() / 2;
    const int x = baseMidX - width() / 2;
    const int y = basePos.y() - height();

    move(x, y);
}

void PopupWidget::setWidget(QWidget *widget)
{
    if (widget) {
        const int count = rootLayout_->count();
        for (int i = 0; i < count; ++i) {
            delete rootLayout_->takeAt(i);
        }

        widget->setParent(this);
        rootLayout_->addWidget(widget);
    }
}

void PopupWidget::mousePressEvent(QMouseEvent *e)
{
    setAttribute(Qt::WA_NoMouseReplay);
    QFrame::mousePressEvent(e);
}

void PopupWidget::resizeEvent(QResizeEvent *e)
{
    return;
    QBitmap bmp(size());
    bmp.fill();

    QPainter painter(&bmp);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(Qt::NoPen);
    painter.setBrush(Qt::black);
    painter.drawRoundedRect(bmp.rect(), 6, 6);

    setMask(bmp);
}

void PopupWidget::locateWithoutParent()
{
    // TODO
    move(0, 0);
}

void PopupWidget::locateWithParent(const QWidget &parent)
{

}

void PopupWidget::locateRelativeTo(QWidget *parent)
{
    if (parent) {
        locateWithParent(*parent);
    } else {
        locateWithoutParent();
    }
}

}
