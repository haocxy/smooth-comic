#include "app-main-quick-window.h"

#include <QCoreApplication>
#include <QKeyEvent>
#include <QQmlEngine>
#include <QQmlContext>

#include "gui-engine.h"


namespace myapp {

AppMainQuickWindow::AppMainQuickWindow(GuiEngine &guiEngine, QWindow *parent)
    : QQuickView(parent)
    , guiEngine_(guiEngine)
{
    QObject::connect(engine(), &QQmlEngine::quit, QCoreApplication::instance(), &QCoreApplication::quit);

    setResizeMode(QQuickView::SizeRootObjectToView);
    engine()->addImportPath("qrc:/qml");
    engine()->rootContext()->setContextProperty("$engine", &guiEngine_);
    setSource(QUrl("qrc:/qml/myapp/MainWindow.qml"));
}

AppMainQuickWindow::~AppMainQuickWindow()
{
}

void AppMainQuickWindow::keyReleaseEvent(QKeyEvent *e)
{
    if (e->key() != Qt::Key_Back) {
        QQuickView::keyReleaseEvent(e);
    } else {
        emit guiEngine_.keyBackReleased();
    }
}

}
