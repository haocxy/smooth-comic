#include "app-main-quick-window.h"

#include <QQmlEngine>
#include <QQmlContext>

#include "gui-engine.h"


namespace myapp {

AppMainQuickWindow::AppMainQuickWindow(GuiEngine &guiEngine, QWindow *parent)
    : QQuickView(parent)
    , guiEngine_(guiEngine)
{
    setResizeMode(QQuickView::SizeRootObjectToView);
    engine()->addImportPath("qrc:/qml");
    engine()->rootContext()->setContextProperty("$engine", &guiEngine_);
    setSource(QUrl("qrc:/qml/myapp/MainWindow.qml"));
}

AppMainQuickWindow::~AppMainQuickWindow()
{
}

}
