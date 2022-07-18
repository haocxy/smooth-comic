#pragma once

#include <QFrame>
#include <QPointer>
#include <QLabel>
#include <QBoxLayout>

#include "gui-util/popup-layer/popup-layer-widget.h"

#include "core/declare_ptr.h"


namespace Ui {
class ScaleSettingPopup;
}

namespace myapp {

class Controller;

class ControllButton;


class ScaleSettingPopup : public PopupLayerWidget {
    Q_OBJECT
public:
    explicit ScaleSettingPopup(Controller &controller, PopupLayer &popupLayer);

    virtual ~ScaleSettingPopup();

private:
    void bindScaleModeButtons();

    void updateCurPercent(float scale);

    bool hasAutoScaleRadioChecked() const;

    void updateZoomButtonsEnabled();

    void setZoomButtonsEnabled(bool enabled);

private:
    Controller &controller_;

    DeclarePtr<Ui::ScaleSettingPopup> ui_;

    QPointer<QHBoxLayout> controllAreaLayout_;

    QPointer<ControllButton> btnZoomOut_;

    QPointer<QLabel> labelCurPercent_;

    QPointer<ControllButton> btnZoomIn_;
};

}
