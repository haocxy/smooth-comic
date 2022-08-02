#include "gui-engine.h"

#include <QDebug>


namespace myapp {

GuiEngine::GuiEngine(QObject *parent)
    : QObject(parent)
{
    qDebug() << "GuiEngine::GuiEngin()";
}

GuiEngine::~GuiEngine()
{
    qDebug() << "GuiEngine::~GuiEngine()";
}

void GuiEngine::testCallFromQML(const QString &msg)
{
    qDebug() << "GuiEngine::testCallFromQML: =====> " << msg;
}

}
