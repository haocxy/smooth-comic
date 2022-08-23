#pragma once

#include <QQuickWidget>
#include <QPointer>


namespace myapp {

class AppQmlEngine;

class AppQuickWidget : public QQuickWidget {
    Q_OBJECT
public:
    explicit AppQuickWidget(AppQmlEngine &appQmlEngine, QWidget &realWindow, QWidget *parent = nullptr);

    virtual ~AppQuickWidget();

    void updateWindowState(const Qt::WindowStates &states);

    QQuickItem *windowItem() {
        return windowItem_;
    }

    QQuickItem *windowMaxButton() {
        return windowMaxButton_;
    }

private:
    AppQmlEngine &appQmlEngine_;
    QPointer<QQuickItem> windowItem_;
    QPointer<QQuickItem> windowMaxButton_;
};

}
