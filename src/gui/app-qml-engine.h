#pragma once

#include <QQmlEngine>

class QKeyEvent;
class QQuickItem;


namespace myapp {

class GuiEngine;


class AppQmlEngine : public QQmlEngine {
    Q_OBJECT
public:
    explicit AppQmlEngine(GuiEngine &guiEngine, QObject *parent = nullptr);

    bool handleKeyRelease(QKeyEvent *e);

    template <typename ViewType>
    void initView(ViewType &view) {
        view.setResizeMode(ViewType::SizeRootObjectToView);
        view.setSource(firstQmlFile());
    }

    static void updateWindowState(QQuickItem *window, Qt::WindowStates states);

private:
    QUrl firstQmlFile() const;

private:
    GuiEngine &guiEngine_;
};

}
