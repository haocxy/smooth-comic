#pragma once

#include "core/declare_ptr.h"

#include "gui-util/popup_widget.h"


namespace Ui {
class ScaleSetting;
}

namespace myapp {

class ScaleSettingPopup : public PopupWidget {
    Q_OBJECT
public:
    explicit ScaleSettingPopup(QWidget *parent = nullptr);

    virtual ~ScaleSettingPopup();

private:
    DeclarePtr<Ui::ScaleSetting> ui_;
};

}
