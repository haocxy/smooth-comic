#include "controll-item.h"


#include "gui-util/controll-button.h"


namespace myapp {

ControllItem::ControllItem(FontIconEnum icon, const QString &text, QWidget *parent)
    : QWidget(parent)
{
    QSizePolicy sp{ sizePolicy() };
    sp.setHorizontalPolicy(QSizePolicy::Policy::Fixed);
    sp.setVerticalPolicy(QSizePolicy::Policy::Fixed);
    setSizePolicy(sp);

    layout_ = new QVBoxLayout(this);
    setLayout(layout_);

    button_ = new ControllButton(icon, this);
    layout_->addWidget(button_);

    text_ = new QLabel(text);
    layout_->addWidget(text_);
}

ControllItem::~ControllItem()
{
}

void ControllItem::setIcon(FontIconEnum icon)
{
    button_->setIcon(icon);
}

}
