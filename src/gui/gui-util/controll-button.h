#pragma once

#include <QPushButton>

#include "gui-util/font-icon-enum.h"


namespace myapp {

class ControllButton : public QPushButton {
    Q_OBJECT
public:
    explicit ControllButton(FontIconEnum icon, QWidget *parent = nullptr);

    virtual ~ControllButton();

    void setIcon(FontIconEnum icon);

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
