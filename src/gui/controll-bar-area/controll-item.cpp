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
    layout_->setSpacing(5);
    setLayout(layout_);

    button_ = new ControllButton(icon, this);
    layout_->addWidget(button_);

    label_ = new QLabel(text);
    label_->setAlignment(Qt::AlignCenter);
    layout_->addWidget(label_);

    setShowLabel(false);

    connect(button_, &QPushButton::clicked, this, &ControllItem::clicked);
}

ControllItem::~ControllItem()
{
}

void ControllItem::setIcon(FontIconEnum icon)
{
    button_->setIcon(icon);
}

void ControllItem::setShowLabel(bool showLabel)
{
    if (showLabel) {
        label_->show();
    } else {
        label_->hide();
    }
}

}
