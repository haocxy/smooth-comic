#include "app-qml-engine.h"

#include <QCoreApplication>
#include <QQmlContext>
#include <QKeyEvent>

#include "gui-engine.h"


namespace myapp {

AppQmlEngine::AppQmlEngine(GuiEngine &guiEngine, QObject *parent)
    : QQmlEngine(parent)
    , guiEngine_(guiEngine)
{
    QObject::connect(this, &QQmlEngine::quit, QCoreApplication::instance(), &QCoreApplication::quit);

    addImportPath("qrc:/qml");
    rootContext()->setContextProperty("$engine", &guiEngine_);
}

bool AppQmlEngine::handleKeyRelease(QKeyEvent *e)
{
    if (e->key() != Qt::Key_Back) {
        return false;
    } else {
        emit guiEngine_.keyBackReleased();
        return true;
    }
}

QUrl AppQmlEngine::firstQmlFile() const
{
    return QUrl("qrc:/qml/myapp/MainWindow.qml");
}

}
