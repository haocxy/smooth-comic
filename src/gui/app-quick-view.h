#pragma once

#include <QQuickView>


namespace myapp {

class AppQmlEngine;

class AppQuickView : public QQuickView {
    Q_OBJECT
public:
    explicit AppQuickView(AppQmlEngine &appQmlEngine, QWindow *parent = nullptr);

    virtual ~AppQuickView();

protected:
    virtual void keyReleaseEvent(QKeyEvent *e) override;

private:
    void updateWindowState();

private:
    AppQmlEngine &appQmlEngine_;
};

}
