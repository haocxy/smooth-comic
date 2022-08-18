#pragma once

#include <QQuickView>


namespace myapp {

class GuiEngine;

class AppMainQuickWindow : public QQuickView {
    Q_OBJECT
public:
    explicit AppMainQuickWindow(GuiEngine &guiEngine, QWindow *parent = nullptr);

    virtual ~AppMainQuickWindow();

protected:
    virtual void keyReleaseEvent(QKeyEvent *e) override;

private:
    GuiEngine &guiEngine_;
};

}
