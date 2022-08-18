#pragma once

#include <QQuickView>


namespace myapp {

class GuiEngine;

class AppMainQuickWindow : public QQuickView {
    Q_OBJECT
public:
    explicit AppMainQuickWindow(GuiEngine &guiEngine, QWindow *parent = nullptr);

    virtual ~AppMainQuickWindow();

private:
    GuiEngine &guiEngine_;
};

}
