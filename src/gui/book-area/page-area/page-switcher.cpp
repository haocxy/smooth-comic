#include "page-switcher.h"

#include <QPainter>

#include "core/debug-option.h"
#include "core/logger.h"

#include "book/book.h"

#include "controller/controller.h"

#include "page-director.h"


namespace myapp {

static DebugOption<bool> dopLog("log.need.page-switcher", true,
    "Is log for page switcher needed?");

using logger::gLogger;


PageSwitcher::PageSwitcher(Controller &controller, QWidget *parent)
    : QWidget(parent)
    , controller_(controller)
    , handle_(*this)
{
    director_ = new PageDirector(controller_, this);
}

PageSwitcher::~PageSwitcher()
{

}

void PageSwitcher::resizeEvent(QResizeEvent *e)
{
    director_->updateShowSize(size());
}

void PageSwitcher::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);
    director_->draw(painter);
}

}
