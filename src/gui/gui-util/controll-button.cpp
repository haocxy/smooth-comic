#include "controll-button.h"


namespace myapp {

ControllButton::ControllButton(FontIconEnum icon, QWidget *parent)
    : QPushButton(QString(QChar(char16_t(icon))))
{
}

ControllButton::~ControllButton()
{
}

}
