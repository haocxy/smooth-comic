#include "scale-setting-popup.h"

#include <cmath>

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

    connect(ui_->radioNoScale, &QRadioButton::clicked, &controller_,
        std::bind(&Controller::cmdSetScaleMode, &controller_, ScaleMode::NoScale));

    connect(ui_->radioScaleBySize, &QRadioButton::clicked, &controller_,
        std::bind(&Controller::cmdSetScaleMode, &controller_, ScaleMode::AutoFitAreaSize));

    connect(ui_->radioScaleByWidth, &QRadioButton::clicked, &controller_,
        std::bind(&Controller::cmdSetScaleMode, &controller_, ScaleMode::AutoFitAreaWidth));

    connect(ui_->radioScaleByHeight, &QRadioButton::clicked, &controller_,
        std::bind(&Controller::cmdSetScaleMode, &controller_, ScaleMode::AutoFitAreaHeight));

    ui_->radioScaleBySize->setChecked(true);
}

ScaleSettingPopup::~ScaleSettingPopup()
{
}

static QString percentToText(int p)
{
    return QString::number(p) + QString("%");
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

