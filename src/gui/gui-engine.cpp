#include "gui-engine.h"

#include <QDebug>

#include "core/system.h"

#include "engine/engine.h"


namespace myapp {

static bool isWindowedPlat() {
    switch (SystemUtil::platformType) {
    case PlatformType::Windows:
        return true;
    case PlatformType::Android:
        return false;
    default:
        return false;
    }
}

GuiEngine::GuiEngine(QObject *parent)
    : QObject(parent)
{
    qDebug() << "GuiEngine::GuiEngine() begin";

    engine_ = new Engine;

    isWindowed_ = isWindowedPlat();

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
