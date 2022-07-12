#pragma once

#include <QFrame>


namespace myapp {

class PopupLayer;


class PopupLayerWidget : public QFrame {
    Q_OBJECT
public:
    explicit PopupLayerWidget(PopupLayer &popupLayer);

    virtual ~PopupLayerWidget();

    bool isPopupVisible() const;

    void setPopupVisible(bool visible);

private:
    PopupLayer &popupLayer_;
};

}
