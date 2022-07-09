#include "book-area.h"

#include <QPainter>

#include "core/logger.h"

#include "controller/controller.h"

#include "status-area/book-status.h"

#include "page-area/page-area.h"


namespace myapp {

using logger::gLogger;


BookArea::BookArea(Controller &controller, QWidget *parent)
    : QSplitter(parent)
    , controller_(controller)
{
    setObjectName("BookArea");

    QSizePolicy sp{ sizePolicy() };
    sp.setVerticalPolicy(QSizePolicy::Policy::Expanding);
    setSizePolicy(sp);

    setChildrenCollapsible(false);

    thumbArea_ = new ThumbArea(controller_, this);
    thumbArea_->setVisible(false);

    mainArea_ = new QWidget(this);

    mainAreaLayout_ = new QVBoxLayout(this);
    mainArea_->setLayout(mainAreaLayout_);

    pageArea_ = new PageArea(controller_, this);
    mainAreaLayout_->addWidget(pageArea_);

    bookStatus_ = new BookStatus(controller_, this);
    mainAreaLayout_->addWidget(bookStatus_);

    addWidget(thumbArea_);
    addWidget(mainArea_);

    setSizes({ 1, 1000 });

    connect(&controller_, &Controller::cmdToggleThumbArea, this, &BookArea::toggleThumbArea);
}

void BookArea::toggleThumbArea()
{
    thumbArea_->setVisible(!thumbArea_->isVisible());
}

}
