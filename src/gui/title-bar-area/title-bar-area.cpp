#include "title-bar-area.h"


namespace myapp {

TitleBarArea::TitleBarArea(QWidget *parent)
    : QWidget(parent)
{
    QSizePolicy sp{ sizePolicy() };
    sp.setVerticalPolicy(QSizePolicy::Policy::Minimum);
    setSizePolicy(sp);

    setContentsMargins(QMargins());

    rootLayout_ = new QHBoxLayout(this);
    rootLayout_->setContentsMargins(QMargins());
    rootLayout_->setSpacing(0);
    setLayout(rootLayout_);

    rootLayout_->addStretch();

    windowFullScreenButton_ = new TitleBarButton(FontIconEnum::WindowFullScreen);
    rootLayout_->addWidget(windowFullScreenButton_);

    windowMinButton_ = new TitleBarButton(FontIconEnum::WindowMinimize);
    rootLayout_->addWidget(windowMinButton_);

    windowMaxButton_ = new TitleBarButton(FontIconEnum::WindowMaximize);
    rootLayout_->addWidget(windowMaxButton_);

    windowCloseButton_ = new TitleBarButton(FontIconEnum::WindowClose);
    rootLayout_->addWidget(windowCloseButton_);
}

TitleBarArea::~TitleBarArea()
{
}

bool TitleBarArea::isWindowMoveAreaContainsLocalPos(const QPoint &localPos)
{
    return childAt(localPos) == nullptr;
}

}
