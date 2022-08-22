#include "app-qml-engine.h"

#include <QCoreApplication>
#include <QQmlContext>
#include <QKeyEvent>
#include <QQuickItem>

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

void AppQmlEngine::updateWindowState(QQuickItem *window, Qt::WindowStates states)
{
    window->setProperty("isMin", states.testFlag(Qt::WindowMaximized));
    window->setProperty("isMax", states.testFlag(Qt::WindowMinimized));
    window->setProperty("isFullScreen", states.testFlag(Qt::WindowFullScreen));
}

QUrl AppQmlEngine::firstQmlFile() const
{
    return QUrl("qrc:/qml/myapp/MainWindow.qml");
}

}
