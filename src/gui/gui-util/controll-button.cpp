#include "controll-button.h"

#include <QPainter>

#include "font-config.h"


namespace myapp {

ControllButton::ControllButton(const FontIcon &icon, QWidget *parent)
    : QPushButton(QString(QChar(icon.code())))
{
    setFlat(true);
    setFont(QFont(icon.fontFamily(), 20));

    constexpr int margin = 4;
    margins_.setTop(margin);
    margins_.setBottom(margin);
    margins_.setLeft(margin);
    margins_.setRight(margin);
}

ControllButton::~ControllButton()
{
}

void ControllButton::setIcon(const FontIcon &icon)
{
    QFont f = font();
    f.setFamily(icon.fontFamily());
    setFont(f);

    setText(QString(QChar(icon.code())));
}

QSize ControllButton::sizeHint() const
{
    return fontMetrics().size(Qt::TextFlag::TextSingleLine, text()).grownBy(margins_);
}

void ControllButton::paintEvent(QPaintEvent *e)
{
    QPainter p(this);

    const QColor colorForNormal{ 0x235a9f };
    const QColor colorForMouseOver{ colorForNormal.lighter(180) };
    const QColor colorForMouseDown{ colorForNormal };

    if (isDown()) {
        p.setPen(colorForMouseDown);
    } else if (mouseOver_) {
        p.setPen(colorForMouseOver);
    } else {
        p.setPen(colorForNormal);
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
