#include "gui-book-manager.h"

#include <QDebug>


namespace myapp {

GuiBookManager::GuiBookManager(QObject *parent)
    : QObject(parent)
{
    qDebug() << "GuiBookManager::GuiBookManager()";
}

GuiBookManager::~GuiBookManager()
{
    qDebug() << "GuiBookManager::~GuiBookManager()";
}

}
