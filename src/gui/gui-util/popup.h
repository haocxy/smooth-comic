#pragma once

#include <QWidget>
#include <QPointer>
#include <QBoxLayout>


namespace myapp
{

class Popup : public QWidget {
    Q_OBJECT
public:
    explicit Popup(QWidget *parent = nullptr);

    virtual ~Popup();

    void locate(QWidget *base);

    void setWidget(QWidget *widget);

    void setLayoutMargin(int margin);

protected:
    virtual void mousePressEvent(QMouseEvent *e) override;

private:
    void locateRelativeTo(QWidget *parent);

    void locateWithoutParent();

    void locateWithParent(const QWidget &parent);

private:
    QPointer<QVBoxLayout> rootLayout_;
};

}
