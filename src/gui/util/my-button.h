#pragma once

#include <QPushButton>


namespace myapp {

class MyButton : public QPushButton {
    Q_OBJECT
public:
    explicit MyButton(const QString &text, QWidget *parent = nullptr);

protected:
    void mouseMoveEvent(QMouseEvent *e) override;

    void enterEvent(QEnterEvent *e) override;

private:

};

}
