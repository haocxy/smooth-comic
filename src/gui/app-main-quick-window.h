#pragma once

#include <QQuickView>


namespace myapp {

class AppQmlEngine;

class AppMainQuickWindow : public QQuickView {
    Q_OBJECT
public:
    explicit AppMainQuickWindow(AppQmlEngine &appQmlEngine, QWindow *parent = nullptr);

    virtual ~AppMainQuickWindow();

protected:
    virtual void keyReleaseEvent(QKeyEvent *e) override;

private:
    AppQmlEngine &appQmlEngine_;
};

}
