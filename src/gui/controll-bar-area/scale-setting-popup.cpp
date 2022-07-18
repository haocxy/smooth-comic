#include "scale-setting-popup.h"

#include <cmath>

#include <QButtonGroup>
#include <QGraphicsDropShadowEffect>

#include "gui-util/controll-button.h"

#include "controller/controller.h"

#include "controll-bar-icons.h"

#include "gen.scale-setting.ui.h"


namespace myapp {

ScaleSettingPopup::ScaleSettingPopup(Controller &controller, PopupLayer &popupLayer)
    : PopupLayerWidget(popupLayer)
    , controller_(controller)
{
    ui_ = new Ui::ScaleSettingPopup;
    ui_->setupUi(this);

    controllAreaLayout_ = new QHBoxLayout(ui_->ControllArea);
    ui_->ControllArea->setLayout(controllAreaLayout_);

    namespace i = ControllBarIcons;

    controllAreaLayout_->addStretch();

    btnZoomOut_ = new ControllButton(i::ZoomOut, this);
    controllAreaLayout_->addWidget(btnZoomOut_);
    connect(btnZoomOut_, &QPushButton::clicked, &controller_, &Controller::cmdZoomOut);

    labelCurPercent_ = new QLabel(this);
    labelCurPercent_->setMinimumWidth(32);
    labelCurPercent_->setAlignment(Qt::AlignCenter);
    controllAreaLayout_->addWidget(labelCurPercent_);

    btnZoomIn_ = new ControllButton(i::ZoomIn, this);
    controllAreaLayout_->addWidget(btnZoomIn_);
    connect(btnZoomIn_, &QPushButton::clicked, &controller_, &Controller::cmdZoomIn);

    controllAreaLayout_->addStretch();

    const int shadowRadius = 20;

    QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect(this);
    shadow->setBlurRadius(shadowRadius);
    shadow->setOffset(0, 0);
    setGraphicsEffect(shadow);

    bindScaleModeButtons();

    connect(&controller_, &Controller::sigScaleUpdated, this, &ScaleSettingPopup::updateCurPercent);


    // 最后选择以确保需要的事件已经绑定
    ui_->radioScaleBySize->setChecked(true);

    updateZoomButtonsEnabled();
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

    auto bindMode = [this, group](QRadioButton *button, ScaleMode mode) {
        group->addButton(button);
        connect(button, &QRadioButton::clicked, &controller_,
            std::bind(&Controller::cmdSetScaleMode, &controller_, mode));
    };

    using e = ScaleMode;

    bindMode(ui_->radioRawSize, e::RawSize);

    bindMode(ui_->radioScaleBySize, e::AutoFitAreaSize);

    bindMode(ui_->radioScaleByWidth, e::AutoFitAreaWidth);

    bindMode(ui_->radioScaleByHeight, e::AutoFitAreaHeight);

    bindMode(ui_->radioFixWidthRatio, e::FixWidthByRatio);

    bindMode(ui_->radioFixHeightRatio, e::FixHeightByRatio);

    bindMode(ui_->radioFixWidthPx, e::FixWidthByPixel);

    bindMode(ui_->radioFixHeightPx, e::FixHeightByPixel);


    auto bindZoomEnabled = [this](QRadioButton *button) {
        connect(button, &QRadioButton::clicked, this,
            &ScaleSettingPopup::updateZoomButtonsEnabled);
    };

    bindZoomEnabled(ui_->radioRawSize);

    bindZoomEnabled(ui_->radioScaleBySize);

    bindZoomEnabled(ui_->radioScaleByWidth);

    bindZoomEnabled(ui_->radioScaleByHeight);

    bindZoomEnabled(ui_->radioFixWidthRatio);

    bindZoomEnabled(ui_->radioFixHeightRatio);

    bindZoomEnabled(ui_->radioFixWidthPx);

    bindZoomEnabled(ui_->radioFixHeightPx);
}

void ScaleSettingPopup::updateCurPercent(float scale)
{
    const int percent = std::round(scale * 100.0f);
    labelCurPercent_->setText(percentToText(percent));
}

bool ScaleSettingPopup::hasAutoScaleRadioChecked() const
{
    if (ui_->radioRawSize->isChecked()) {
        return true;
    }

    if (ui_->radioScaleBySize->isChecked()) {
        return true;
    }

    if (ui_->radioScaleByWidth->isChecked()) {
        return true;
    }

    if (ui_->radioScaleByHeight->isChecked()) {
        return true;
    }

    return false;
}

void ScaleSettingPopup::updateZoomButtonsEnabled()
{
    setZoomButtonsEnabled(!hasAutoScaleRadioChecked());
}

void ScaleSettingPopup::setZoomButtonsEnabled(bool enabled)
{
    btnZoomOut_->setEnabled(enabled);
    btnZoomIn_->setEnabled(enabled);
}

}

