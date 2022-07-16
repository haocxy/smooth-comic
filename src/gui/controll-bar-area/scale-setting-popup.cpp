#include "scale-setting-popup.h"

#include <cmath>

#include <QButtonGroup>
#include <QGraphicsDropShadowEffect>

#include "controller/controller.h"

#include "gen.scale-setting.ui.h"


namespace myapp {

ScaleSettingPopup::ScaleSettingPopup(Controller &controller, PopupLayer &popupLayer)
    : PopupLayerWidget(popupLayer)
    , controller_(controller)
{
    ui_ = new Ui::ScaleSettingPopup;
    ui_->setupUi(this);


    const int shadowRadius = 20;

    QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect(this);
    shadow->setBlurRadius(shadowRadius);
    shadow->setOffset(0, 0);
    setGraphicsEffect(shadow);


    connect(&controller_, &Controller::sigScaleRangeUpdated, this,
        &ScaleSettingPopup::updateScaleRange);

    bindScaleModeButtons();

    ui_->radioScaleBySize->setChecked(true);
}

ScaleSettingPopup::~ScaleSettingPopup()
{
}

static QString percentToText(int p)
{
    return QString::number(p) + QString("%");
}

void ScaleSettingPopup::bindScaleModeButtons()
{
    QButtonGroup *group = new QButtonGroup(this);

    auto bind = [this, group](QRadioButton *button, ScaleMode mode) {
        group->addButton(button);
        connect(button, &QRadioButton::clicked, &controller_,
            std::bind(&Controller::cmdSetScaleMode, &controller_, mode));
    };

    using e = ScaleMode;

    bind(ui_->radioRawSize, e::RawSize);

    bind(ui_->radioScaleBySize, e::AutoFitAreaSize);

    bind(ui_->radioScaleByWidth, e::AutoFitAreaWidth);

    bind(ui_->radioScaleByHeight, e::AutoFitAreaHeight);

    bind(ui_->radioFixWidthRatio, e::FixWidthByRatio);

    bind(ui_->radioFixHeightRatio, e::FixHeightByRatio);

    bind(ui_->radioFixWidthPx, e::FixWidthByPixel);

    bind(ui_->radioFixHeightPx, e::FixHeightByPixel);
}

void ScaleSettingPopup::updateScaleRange(float minScale, float maxScale)
{
    const int minPercent = std::floor(minScale * 100);
    const int maxPercent = std::ceil(maxScale * 100);

    ui_->scaleSlider->setMinimum(minPercent);
    ui_->scaleSlider->setMaximum(maxPercent);

    ui_->scaleMinValue->setText(percentToText(minPercent));
    ui_->scaleMaxValue->setText(percentToText(maxPercent));
}

void ScaleSettingPopup::updateScale(float scale)
{
    const int percent = std::round(scale);

    ui_->scaleSlider->setValue(percent);
}

}

