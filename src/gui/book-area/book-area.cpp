#include "book-area.h"

#include <QPainter>

#include "core/logger.h"

#include "status-area/book-status.h"

#include "page-area/page-area.h"


namespace myapp {

using logger::gLogger;


BookArea::BookArea(Controller &controller, QWidget *parent)
    : QSplitter(parent)
{
    QSizePolicy sp{ sizePolicy() };
    sp.setVerticalPolicy(QSizePolicy::Policy::Expanding);
    setSizePolicy(sp);

    setChildrenCollapsible(false);

    thumbArea_ = new ThumbArea(controller, this);

    mainArea_ = new QWidget(this);

    mainAreaLayout_ = new QVBoxLayout(this);
    mainArea_->setLayout(mainAreaLayout_);

    pageArea_ = new PageArea(controller, this);
    mainAreaLayout_->addWidget(pageArea_);

    bookStatus_ = new BookStatus(controller, this);
    mainAreaLayout_->addWidget(bookStatus_);

    addWidget(thumbArea_);
    addWidget(mainArea_);

    setSizes({ 1, 1000 });
}

}
