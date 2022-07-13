#pragma once

#include <QFrame>

#include "gui-util/popup-layer/popup-layer-widget.h"

#include "core/declare_ptr.h"


namespace Ui {
class ScaleSettingPopup;
}

namespace myapp {

class ScaleSettingPopup : public PopupLayerWidget {
    Q_OBJECT
public:
    explicit ScaleSettingPopup(PopupLayer &popupLayer);

    virtual ~ScaleSettingPopup();

private:
    DeclarePtr<Ui::ScaleSettingPopup> ui_;
};

}
