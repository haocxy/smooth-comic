#pragma once

#include <optional>

#include <QFrame>
#include <QPointer>


namespace myapp {

class PopupLayer;


class PopupLayerWidget : public QFrame {
    Q_OBJECT
public:

    enum class PinType {
        ToWidget,
    };

    explicit PopupLayerWidget(PopupLayer &popupLayer);

    virtual ~PopupLayerWidget();

    bool isPopupVisible() const;

    void setPopupVisible(bool visible);

    void pinTo(QWidget *target, int padding);

    const std::optional<PinType> &pinType() const {
        return pinType_;
    }

    const QWidget *pinTarget() const {
        return pinTarget_;
    }

    int pinPadding() const {
        return pinPadding_;
    }

private:
    PopupLayer &popupLayer_;

private:
    std::optional<PinType> pinType_;
    QPointer<QWidget> pinTarget_;
    int pinPadding_{ 0 };
};

}
