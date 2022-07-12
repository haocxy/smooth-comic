#include "popup-layer.h"

#include <QPainter>
#include <QMouseEvent>

#include "popup-layer-layout.h"


namespace myapp {

PopupLayer::PopupLayer(QWidget *parent)
    : QWidget(parent)
{
    QSizePolicy sp{ sizePolicy() };
    sp.setHorizontalPolicy(QSizePolicy::Policy::Ignored);
    sp.setVerticalPolicy(QSizePolicy::Policy::Ignored);
    setSizePolicy(sp);

    layout_ = new PopupLayerLayout(this);
    setLayout(layout_);
}

PopupLayer::~PopupLayer()
{
}

void PopupLayer::addPopup(QWidget *popup)
{
    layout_->addWidget(popup);
}

void PopupLayer::mousePressEvent(QMouseEvent *e)
{
    if (!childAt(e->pos())) {
        hide();
    }
}

}
