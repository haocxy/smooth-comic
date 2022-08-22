#pragma once

#include <QQuickWidget>


namespace myapp {

class AppQmlEngine;

class AppQuickWidget : public QQuickWidget {
    Q_OBJECT
public:
    explicit AppQuickWidget(AppQmlEngine &appQmlEngine, QWidget *parent = nullptr);

    virtual ~AppQuickWidget();

protected:
    virtual void keyReleaseEvent(QKeyEvent *e) override;

    virtual void changeEvent(QEvent *e) override;

private:
    void updateWindowState();

private:
    AppQmlEngine &appQmlEngine_;
};

}
