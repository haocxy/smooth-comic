#pragma once

#include <QPointer>

#include "core/declare_ptr.h"
#include "gui-util/frameless-window.h"


namespace myapp {

class AppQmlEngine;

class AppQuickWidget;

class AppQuickWindow : public FramelessWindow {
    Q_OBJECT
public:
    explicit AppQuickWindow(AppQmlEngine &appQmlEngine, QWidget *parent = nullptr);

protected:
    virtual bool hasWindowMaxButton() const override;

    virtual void setWindowMaxButtonHover(bool hover) override;

    virtual void onWindowMaxButtonClicked() override;

    virtual bool isWindowMaxButtonContainsGlobalPos(const QPoint &gpos) const override;

    virtual bool isWindowMoveAreaContainsGlobalPos(const QPoint &gpos) const override;

    virtual void keyReleaseEvent(QKeyEvent *e) override;

    virtual void changeEvent(QEvent *e) override;

private:
    void updateWindowState();

private:
    AppQmlEngine &appQmlEngine_;
    DeclarePtr<AppQuickWidget> appWidget_;
};

}
