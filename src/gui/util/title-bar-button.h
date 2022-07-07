#pragma once

#include <QPushButton>

#include "font-icon-enum.h"


namespace myapp {

class TitleBarButton : public QPushButton {
    Q_OBJECT
public:
    explicit TitleBarButton(const QString &text, QWidget *parent = nullptr);

    TitleBarButton(FontIconEnum fontIcon, QWidget *parent = nullptr);

    TitleBarButton(const QIcon &icon, const QString &text, QWidget *parent = nullptr);

    explicit TitleBarButton(QWidget *parent = nullptr);

    bool mouseOver() const {
        return mouseOver_;
    }

    void setMouseOver(bool mouseOver);

protected:
    virtual QSize sizeHint() const override;

    virtual void paintEvent(QPaintEvent *e) override;

    virtual void enterEvent(QEnterEvent *e) override;

    virtual void leaveEvent(QEvent *e) override;

private:
    void init();

private:
    bool mouseOver_{ false };
    QMargins margins_;
};

}
