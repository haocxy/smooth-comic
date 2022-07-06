#include "my-button.h"


namespace myapp{

MyButton::MyButton(const QString &text, QWidget *parent)
    : QPushButton(text, parent)
{
    ((QPushButton *)this);
}

void MyButton::mouseMoveEvent(QMouseEvent *e)
{
}

void MyButton::enterEvent(QEnterEvent *e)
{
}

}
