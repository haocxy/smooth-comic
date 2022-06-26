#include "page-viewer.h"

#include <QPainter>

#include "page-switcher.h"
#include "page-controll-layer.h"


namespace myapp {

PageViewerWidget::PageViewerWidget(Book &book, QWidget *parent)
    : QWidget(parent)
    , book_(book)
{
    layout_ = new QStackedLayout(this);
    setLayout(layout_);

    pageSwitcher_ = new PageSwitcher(book_, this);
    layout_->addWidget(pageSwitcher_);

    pageControllLayer_ = new PageControllLayer(this);
    layout_->addWidget(pageControllLayer_);
}

}
