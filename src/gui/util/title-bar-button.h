#pragma once

#include <QPushButton>


namespace myapp {

class TitleBarButton : public QPushButton {
    Q_OBJECT

    Q_PROPERTY(bool mouseOver READ mouseOver WRITE setMouseOver)
public:
    explicit TitleBarButton(const QString &text, QWidget *parent = nullptr);

    TitleBarButton(const QIcon &icon, const QString &text, QWidget *parent = nullptr);

    explicit TitleBarButton(QWidget *parent = nullptr);

    bool mouseOver() const {
        return mouseOver_;
    }

    void setMouseOver(bool mouseOver) {
        if (mouseOver_ != mouseOver) {
            mouseOver_ = mouseOver;
            update();
        }
    }

private:
    void init();

private:
    bool mouseOver_{ false };
};

}
