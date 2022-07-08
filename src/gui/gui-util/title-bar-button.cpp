#include "title-bar-button.h"

#include <QPainter>
#include <QStyle>

#include "gui-util/property-name.h"

#include "font-icon-enum.h"


namespace myapp {

namespace prop {

using PropName = gui::PropertyName;


}

TitleBarButton::TitleBarButton(const QString &text, QWidget *parent)
    : QPushButton(text, parent)
{
    init();
}

TitleBarButton::TitleBarButton(FontIconEnum fontIcon, QWidget *parent)
    : QPushButton(QString(QChar(char16_t(fontIcon))), parent)
{
    init();
}

TitleBarButton::TitleBarButton(const QIcon &icon, const QString &text, QWidget *parent)
    : QPushButton(icon, text, parent)
{
    init();
}

TitleBarButton::TitleBarButton(QWidget *parent)
    : QPushButton(parent)
{
    init();
}

void TitleBarButton::setFontIcon(FontIconEnum fontIcon)
{
    setText(QString(QChar(char16_t(fontIcon))));
}

void TitleBarButton::setMouseOver(bool mouseOver) {
    if (mouseOver_ != mouseOver) {
        mouseOver_ = mouseOver;
        update();
    }
}

QSize TitleBarButton::sizeHint() const
{
    return fontMetrics().size(Qt::TextFlag::TextSingleLine, text()).grownBy(margins_);
}

void TitleBarButton::paintEvent(QPaintEvent *e)
{
    QPainter p(this);

    if (mouseOver_) {
        p.fillRect(rect(), palette().base().color().lighter());
    }

    style()->drawItemText(&p, rect(), Qt::AlignCenter, palette(), true, text());
}

void TitleBarButton::enterEvent(QEnterEvent *e)
{
    setMouseOver(true);
}

void TitleBarButton::leaveEvent(QEvent *e)
{
    setMouseOver(false);
}

void TitleBarButton::init()
{
    setFlat(true);

    margins_.setTop(10);
    margins_.setBottom(10);
    margins_.setLeft(20);
    margins_.setRight(20);

    setFont(QFont("Segoe Fluent Icons", 8));
}

}
