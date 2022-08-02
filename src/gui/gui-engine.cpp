#include "gui-engine.h"

#include <QDebug>

#include "engine/engine.h"


namespace myapp {

GuiEngine::GuiEngine(QObject *parent)
    : QObject(parent)
{
    qDebug() << "GuiEngine::GuiEngine() begin";

    engine_ = new Engine;

    qDebug() << "GuiEngine::GuiEngine() end";
}

GuiEngine::~GuiEngine()
{
    qDebug() << "GuiEngine::~GuiEngine()";
}

void GuiEngine::testCallFromQML(const QString &msg)
{
    qDebug() << "GuiEngine::testCallFromQML: =====> " << msg;
}

Engine &GuiEngine::engine()
{
    return *engine_;
}

}
