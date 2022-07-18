#include "page-switcher.h"

#include <QPainter>

#include "core/debug-option.h"
#include "core/logger.h"

#include "book/book.h"

#include "controller/controller.h"

#include "page-scene.h"


namespace myapp {

static DebugOption<bool> dopLog("log.need.page-switcher", true,
    "Is log for page switcher needed?");

using logger::gLogger;


PageSwitcher::PageSwitcher(Controller &controller, PageScene &scene, QWidget *parent)
    : QWidget(parent)
    , controller_(controller)
    , scene_(scene)
    , handle_(*this)
{
    connect(&scene_, &PageScene::cmdUpdate, this, [this] {
        update();
    });
}

PageSwitcher::~PageSwitcher()
{

}

void PageSwitcher::resizeEvent(QResizeEvent *e)
{
    scene_.updateSceneSize(size());
}

void PageSwitcher::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);
    scene_.draw(painter);
}

}
