#include "title-bar-area.h"

#include "font-icon-enum.h"


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

    windowMaxButton_ = new TitleBarButton(FontIconEnum::WindowMaximize);
    rootLayout_->addWidget(windowMaxButton_);

    windowCloseButton_ = new TitleBarButton(FontIconEnum::WindowClose);
    rootLayout_->addWidget(windowCloseButton_);
}

TitleBarArea::~TitleBarArea()
{
}

bool TitleBarArea::isWindowMoveAreaContainsGlobalPos(const QPoint &gpos)
{
    const QPoint localPos = mapFromGlobal(gpos);
    if (childAt(localPos)) {
        return false;
    } else {
        return true;
    }
}

}
