#pragma once

#include <QFrame>
#include <QPointer>
#include <QBoxLayout>


namespace myapp
{

class PopupWidget : public QFrame {
    Q_OBJECT
public:
    explicit PopupWidget(QWidget *parent = nullptr);

    virtual ~PopupWidget();

    void locate(QWidget *base);

    void setWidget(QWidget *widget);

protected:
    virtual void mousePressEvent(QMouseEvent *e) override;

    virtual void resizeEvent(QResizeEvent *e) override;

private:
    void locateRelativeTo(QWidget *parent);

    void locateWithoutParent();

    void locateWithParent(const QWidget &parent);

private:
    QPointer<QVBoxLayout> rootLayout_;
};

}
