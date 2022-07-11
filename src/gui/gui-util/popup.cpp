#include "popup.h"


namespace myapp
{

Popup::Popup(QWidget *parent)
    : QWidget(parent) {

    setWindowFlag(Qt::Popup);
    setWindowFlag(Qt::FramelessWindowHint);
    setWindowFlag(Qt::NoDropShadowWindowHint);

    setAttribute(Qt::WA_TranslucentBackground);

    rootLayout_ = new QVBoxLayout(this);
    setLayout(rootLayout_);
}

Popup::~Popup()
{
}

void Popup::locate(QWidget *base)
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

void Popup::setWidget(QWidget *widget)
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

void Popup::setLayoutMargin(int margin)
{
    rootLayout_->setContentsMargins(QMargins(margin, margin, margin, margin));
}

void Popup::mousePressEvent(QMouseEvent *e)
{
    setAttribute(Qt::WA_NoMouseReplay);
    QWidget::mousePressEvent(e);
}

void Popup::locateWithoutParent()
{
    // TODO
    move(0, 0);
}

void Popup::locateWithParent(const QWidget &parent)
{

}

void Popup::locateRelativeTo(QWidget *parent)
{
    if (parent) {
        locateWithParent(*parent);
    } else {
        locateWithoutParent();
    }
}

}
