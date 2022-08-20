#include "page-reader-quick-item.h"



#include "page-scene.h"


namespace myapp {

PageReaderQuickItem::PageReaderQuickItem(QQuickItem *parent)
    : QQuickPaintedItem(parent)
{

}

PageReaderQuickItem::~PageReaderQuickItem()
{

}

void PageReaderQuickItem::paint(QPainter *painter)
{
    if (scene_) {
        scene_->draw(*painter);
    }
}

void PageReaderQuickItem::setController(Controller *controller)
{
    if (controller_) {
        throw std::logic_error("cannot set controller again");
    }

    controller_ = controller;

    scene_ = new PageScene(*controller);
    connect(scene_, &PageScene::cmdUpdate, this, [this] { update(); });
    connect(this, &PageReaderQuickItem::widthChanged, this, &PageReaderQuickItem::updateSceneSize);
    connect(this, &PageReaderQuickItem::heightChanged, this, &PageReaderQuickItem::updateSceneSize);

    emit controllerChanged();
}

void PageReaderQuickItem::updateSceneSize()
{
    if (scene_) {
        const QSizeF s = this->size();
        scene_->updateSceneSize(QSize(s.width(), s.height()));
    }
}

}
