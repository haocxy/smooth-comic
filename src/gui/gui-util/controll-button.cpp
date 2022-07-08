#include "controll-button.h"

#include <QPainter>

#include "font-config.h"


namespace myapp {

ControllButton::ControllButton(FontIconEnum icon, QWidget *parent)
    : QPushButton(QString(QChar(char16_t(icon))))
{
    setFlat(true);
    setFont(QFont(FontConfig::UiFontFamily, 20));

    margins_.setTop(10);
    margins_.setBottom(10);
    margins_.setLeft(10);
    margins_.setRight(10);
}

ControllButton::~ControllButton()
{
}

void ControllButton::setIcon(FontIconEnum icon)
{
    setText(QString(QChar(char16_t(icon))));
}

QSize ControllButton::sizeHint() const
{
    return fontMetrics().size(Qt::TextFlag::TextSingleLine, text()).grownBy(margins_);
}

void ControllButton::paintEvent(QPaintEvent *e)
{
    QPainter p(this);

    const QColor colorForMouseOver{ Qt::red };
    const QColor colorForMouseDown{ colorForMouseOver.darker(150) };

    if (isDown()) {
        p.setPen(colorForMouseDown);
    } else if (mouseOver_) {
        p.setPen(colorForMouseOver);
    }

    p.drawText(rect(), Qt::AlignCenter, text());
}

void ControllButton::enterEvent(QEnterEvent *e)
{
    setMouseOver(true);
}

void ControllButton::leaveEvent(QEvent *e)
{
    setMouseOver(false);
}

void ControllButton::setMouseOver(bool mouseOver)
{
    if (mouseOver_ != mouseOver) {
        mouseOver_ = mouseOver;
        update();
    }
}

}
