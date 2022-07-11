#include "scale-setting-popup.h"

#include "gen.scale-setting.ui.h"


namespace myapp {

ScaleSettingPopup::ScaleSettingPopup(QWidget *parent)
    : Popup(parent)
{
    rootWidget_ = new QFrame(this);
    rootWidget_->setObjectName("ScaleSettingPopup");

    ui_ = new Ui::ScaleSetting;
    ui_->setupUi(rootWidget_);

    setWidget(rootWidget_);
}

ScaleSettingPopup::~ScaleSettingPopup()
{
}

}

