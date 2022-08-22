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

private:
    AppQmlEngine &appQmlEngine_;
};

}
