#pragma once

#include <QPushButton>

#include "font-icon.h"


namespace myapp {

class ControllButton : public QPushButton {
    Q_OBJECT
public:
    explicit ControllButton(const FontIcon &iconicon, QWidget *parent = nullptr);

    virtual ~ControllButton();

    void setIcon(const FontIcon &icon);

    virtual QSize sizeHint() const override;

protected:
    virtual void paintEvent(QPaintEvent *e) override;

    virtual void enterEvent(QEnterEvent *e) override;

    virtual void leaveEvent(QEvent *e) override;

private:
    void setMouseOver(bool mouseOver);

private:
    QMargins margins_;
    bool mouseOver_{ false };
};

}
