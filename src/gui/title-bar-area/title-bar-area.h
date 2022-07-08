#pragma once

#include <QWidget>
#include <QPointer>
#include <QBoxLayout>

#include "gui-util/title-bar-button.h"


namespace myapp {

class TitleBarArea : public QWidget {
    Q_OBJECT
public:
    explicit TitleBarArea(QWidget *parent = nullptr);

    virtual ~TitleBarArea();

    TitleBarButton *windowFullScreenButton() {
        return windowFullScreenButton_;
    }

    TitleBarButton *windowMinButton() {
        return windowMinButton_;
    }

    TitleBarButton *windowMaxButton() {
        return windowMaxButton_;
    }

    TitleBarButton *windowCloseButton() {
        return windowCloseButton_;
    }

    bool isWindowMoveAreaContainsLocalPos(const QPoint &localPos);

private:
    QPointer<QHBoxLayout> rootLayout_;
    QPointer<TitleBarButton> windowFullScreenButton_;
    QPointer<TitleBarButton> windowMinButton_;
    QPointer<TitleBarButton> windowMaxButton_;
    QPointer<TitleBarButton> windowCloseButton_;
};

}
