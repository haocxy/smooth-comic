#pragma once

#include <QPointer>

#include "gui-util/frameless-window.h"


namespace myapp {

class GuiEngine;

class AppWidget;

class AppWindow : public FramelessWindow {
    Q_OBJECT
public:
    explicit AppWindow(GuiEngine &engine, QWidget *parent = nullptr);

protected:
    virtual bool hasWindowMaxButton() const override;

    virtual void setWindowMaxButtonHover(bool hover) override;

    virtual void onWindowMaxButtonClicked() override;

    virtual bool isWindowMaxButtonContainsGlobalPos(const QPoint &gpos) const override;

    virtual bool isWindowMoveAreaContainsGlobalPos(const QPoint &gpos) const override;

    virtual void changeEvent(QEvent *e) override;

private:
    void toggleWindowFullScreenAction();

    void toggleWindowMaxAction();

    void switchWindowStateTo(Qt::WindowState newState);

    void onWindowStateChanged(Qt::WindowStates oldStates, Qt::WindowStates newStates);

private:
    QPointer<AppWidget> appWidget_;
    QRect normalSizeGeometry_;
};

}
