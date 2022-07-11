#include "scale-setting-popup.h"

#include <QGraphicsDropShadowEffect>

#include "gen.scale-setting.ui.h"


namespace myapp {

ScaleSettingPopup::ScaleSettingPopup(QWidget *parent)
    : Popup(parent)
{
    rootWidget_ = new QFrame(this);
    rootWidget_->setObjectName("ScaleSettingPopup");

    const int shadowRadius = 20;
    setLayoutMargin(shadowRadius);

    QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect(this);
    shadow->setBlurRadius(shadowRadius);
    shadow->setOffset(0, 0);
    rootWidget_->setGraphicsEffect(shadow);

    ui_ = new Ui::ScaleSetting;
    ui_->setupUi(rootWidget_);

    setWidget(rootWidget_);
}

ScaleSettingPopup::~ScaleSettingPopup()
{
}

}

