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

}

