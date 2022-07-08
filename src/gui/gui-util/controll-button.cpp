#include "controll-button.h"

#include "font-config.h"


namespace myapp {

ControllButton::ControllButton(FontIconEnum icon, QWidget *parent)
    : QPushButton(QString(QChar(char16_t(icon))))
{
    setFont(QFont(FontConfig::UiFontFamily, 20));
}

ControllButton::~ControllButton()
{
}

void ControllButton::setIcon(FontIconEnum icon)
{
    setText(QString(QChar(char16_t(icon))));
}

}
