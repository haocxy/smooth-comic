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

    // 是否处于模态状态
    // 注意！判断的是此时此刻的实际状态
    bool inModalState() const;

private:
    virtual void mousePressEvent(QMouseEvent *e) override;

private:
    QPointer<PopupLayerLayout> layout_;
};

}
