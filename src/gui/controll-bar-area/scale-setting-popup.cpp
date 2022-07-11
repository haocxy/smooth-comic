#include "scale-setting-popup.h"


#include "gen.scale-setting.ui.h"


namespace myapp {

ScaleSettingPopup::ScaleSettingPopup(QWidget *parent)
    : PopupWidget(parent)
{
    ui_ = new Ui::ScaleSetting;
    ui_->setupUi(this);
}

ScaleSettingPopup::~ScaleSettingPopup()
{
}

}

