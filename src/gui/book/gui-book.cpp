#include "gui-book.h"

#include <QDebug>


namespace myapp {

GuiBook::GuiBook(QObject *parent)
    : QObject(parent)
{
    qDebug() << "GuiBook::GuiBook()";
}

GuiBook::~GuiBook()
{
    qDebug() << "GuiBook::~GuiBook()";
}

}
