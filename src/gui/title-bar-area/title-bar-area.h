#pragma once

#include <QWidget>
#include <QPointer>
#include <QBoxLayout>

#include "util/title-bar-button.h"


namespace myapp {

class TitleBarArea : public QWidget {
    Q_OBJECT
public:
    explicit TitleBarArea(QWidget *parent = nullptr);

    virtual ~TitleBarArea();

    TitleBarButton *windowMaxButton() {
        return windowMaxButton_;
    }

    TitleBarButton *windowCloseButton() {
        return windowCloseButton_;
    }

    bool isWindowMoveAreaContainsGlobalPos(const QPoint &gpos);

private:
    QPointer<QHBoxLayout> rootLayout_;
    QPointer<TitleBarButton> windowMaxButton_;
    QPointer<TitleBarButton> windowCloseButton_;
};

}
