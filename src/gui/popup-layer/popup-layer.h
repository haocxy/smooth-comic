#pragma once

#include <QWidget>
#include <QPointer>


namespace myapp {

class PopupLayerLayout;


class PopupLayer : public QWidget {
    Q_OBJECT
public:
    explicit PopupLayer(QWidget *parent = nullptr);

    virtual ~PopupLayer();

    void addPopup(QWidget *popup);

private:
    virtual void mousePressEvent(QMouseEvent *e) override;

private:
    QPointer<PopupLayerLayout> layout_;
};

}
