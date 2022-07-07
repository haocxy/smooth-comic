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

private:
    void toggleWindowFullScreen();

    void toggleWindowMaxAction();



private:
    QPointer<AppWidget> appWidget_;
};

}
