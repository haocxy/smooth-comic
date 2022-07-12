#include "app-widget.h"

#include <QFileDialog>

#include "core/system.h"

#include "book/book.h"

#include "controller/controller.h"

#include "popup-layer/popup-layer.h"
#include "title-bar-area/title-bar-area.h"
#include "book-area/book-area.h"
#include "controll-bar-area/controll-bar-area.h"
#include "controll-bar-area/scale-setting-popup.h"


namespace myapp {

AppWidget::AppWidget(Engine &engine, QWidget *parent)
    : QWidget(parent)
    , engine_(engine)
{
    controller_ = new Controller(engine, this);

    resize(1000, 800);
    setMinimumSize(800, 600);

    initAreas();

    connect(controller_, &Controller::cmdOpenComic, this, &AppWidget::fileOpenAction);

    connect(controller_, &Controller::cmdToggleScaleSettingPopup, this, &AppWidget::toggleScaleSettingPopup);
}

AppWidget::~AppWidget()
{
}

bool AppWidget::isWindowMoveAreaContainsGlobalPos(const QPoint &gpos)
{
    if (titleBarArea_) {
        const QPoint localPos = titleBarArea_->mapFromGlobal(gpos);
        if (titleBarArea_->rect().contains(localPos)) {
            return titleBarArea_->isWindowMoveAreaContainsLocalPos(localPos);
        }
    }

    // 除了窗口标题栏区域之外的其它区域都只有在弹出层激活时才可用于移动窗口
    // 其它区域均用于处理和弹出层相关的逻辑，例如点击菜单之外时关闭弹出层

    if (!popupLayer_->inModalState()) {
        if (controllBarArea_) {
            const QPoint localPos = controllBarArea_->mapFromGlobal(gpos);
            if (controllBarArea_->rect().contains(localPos)) {
                return controllBarArea_->isWindowMoveAreaContainsLocalPos(localPos);
            }
        }
    }

    return false;
}

void AppWidget::initAreas()
{
    setContentsMargins(QMargins());

    layersLayout_ = new QStackedLayout(this);
    layersLayout_->setStackingMode(QStackedLayout::StackingMode::StackAll);
    setLayout(layersLayout_);

    popupLayer_ = new PopupLayer(this);
    layersLayout_->addWidget(popupLayer_);
    popupLayer_->hide();

    popupLayer_->addPopup(new ScaleSettingPopup(this));

    mainLayer_ = new QWidget(this);
    layersLayout_->addWidget(mainLayer_);

    mainLayerLayout_ = new QVBoxLayout(mainLayer_);
    mainLayerLayout_->setSpacing(0);
    mainLayerLayout_->setContentsMargins(QMargins());

    mainLayer_->setLayout(mainLayerLayout_);

    initTitleBarArea();

    initBookArea();

    initControllBarArea();
}

void AppWidget::initTitleBarArea()
{
    titleBarArea_ = new TitleBarArea(this);
    mainLayerLayout_->addWidget(titleBarArea_);
}

void AppWidget::initBookArea()
{
    bookArea_ = new BookArea(*controller_, this);
    mainLayerLayout_->addWidget(bookArea_);
}

void AppWidget::initControllBarArea()
{
    controllBarArea_ = new ControllBarArea(*controller_, this);
    mainLayerLayout_->addWidget(controllBarArea_);
}

void AppWidget::fileOpenAction()
{
    const fs::path home = SystemUtil::userHome();
    const QString defaultDir = QString::fromStdU32String(home.generic_u32string());
    const QString filePath = QFileDialog::getOpenFileName(this, tr("Open Comic"), defaultDir);

    if (QFile(filePath).exists()) {
        controller_->book().open(filePath.toStdU32String());
    }
}

void AppWidget::toggleScaleSettingPopup()
{
    popupLayer_->show();
}

}
