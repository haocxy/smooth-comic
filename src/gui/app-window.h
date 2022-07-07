#pragma once

#include <QPointer>

#include "util/frameless-window.h"


namespace myapp {

class Engine;

class AppWidget;

class AppWindow : public FramelessWindow {
    Q_OBJECT
public:
    explicit AppWindow(Engine &engine, QWidget *parent = nullptr);

protected:
    virtual TitleBarButton *windowMaxButton() override;

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
