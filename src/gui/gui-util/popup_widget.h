#pragma once

#include <QFrame>


namespace myapp
{

class PopupWidget : public QFrame {
    Q_OBJECT
public:
    explicit PopupWidget(QWidget *parent = nullptr);

    virtual ~PopupWidget();

    void locate(QWidget *base);

protected:
    virtual void mousePressEvent(QMouseEvent *e) override;

private:
    void locateRelativeTo(QWidget *parent);

    void locateWithoutParent();

    void locateWithParent(const QWidget &parent);
};

}
