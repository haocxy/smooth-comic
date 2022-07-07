#include "title-bar-button.h"

#include "util/property-name.h"


namespace myapp {

namespace prop {

using PropName = gui::PropertyName;


}

TitleBarButton::TitleBarButton(const QString &text, QWidget *parent)
    : QPushButton(text, parent)
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

void TitleBarButton::setMouseOver(bool mouseOver) {
    if (mouseOver_ != mouseOver) {
        mouseOver_ = mouseOver;
        update();
    }
}

void TitleBarButton::init()
{
    setFlat(true);

    
}

}
