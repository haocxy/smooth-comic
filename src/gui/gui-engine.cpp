#include "gui-engine.h"

#include <QDebug>

#include "core/system.h"

#include "engine/engine.h"


namespace myapp {

GuiEngine::GuiEngine(QObject *parent)
    : QObject(parent)
{
    qDebug() << "GuiEngine::GuiEngine() begin";

    engine_ = new Engine;

    isWindowed_ = SystemUtil::isWindowedPlatform();

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
