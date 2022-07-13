#pragma once

#include <QFrame>

#include "gui-util/popup-layer/popup-layer-widget.h"

#include "core/declare_ptr.h"


namespace Ui {
class ScaleSettingPopup;
}

namespace myapp {

class Controller;


class ScaleSettingPopup : public PopupLayerWidget {
    Q_OBJECT
public:
    explicit ScaleSettingPopup(Controller &controller, PopupLayer &popupLayer);

    virtual ~ScaleSettingPopup();

private:
    void updateScaleRange(float minScale, float maxScale);

    void updateScale(float scale);

private:
    Controller &controller_;

    DeclarePtr<Ui::ScaleSettingPopup> ui_;
};

}
