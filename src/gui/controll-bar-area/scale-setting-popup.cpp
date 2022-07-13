#include "scale-setting-popup.h"

#include <QGraphicsDropShadowEffect>

#include "gen.scale-setting.ui.h"


namespace myapp {

ScaleSettingPopup::ScaleSettingPopup(PopupLayer &popupLayer)
    : PopupLayerWidget(popupLayer)
{
    ui_ = new Ui::ScaleSettingPopup;
    ui_->setupUi(this);

    const int shadowRadius = 20;

    QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect(this);
    shadow->setBlurRadius(shadowRadius);
    shadow->setOffset(0, 0);
    setGraphicsEffect(shadow);
}

ScaleSettingPopup::~ScaleSettingPopup()
{
}

}

