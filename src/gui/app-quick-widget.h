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

    QQuickItem *windowTitleBar() {
        return windowTitleBar_;
    }

    QQuickItem *windowMaxButton() {
        return windowMaxButton_;
    }

    QQuickItem *windowStateSwitcher() {
        return windowStateSwitcher_;
    }

private:
    AppQmlEngine &appQmlEngine_;
    QPointer<QQuickItem> windowTitleBar_;
    QPointer<QQuickItem> windowMaxButton_;
    QPointer<QQuickItem> windowStateSwitcher_;
};

}
