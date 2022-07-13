#include "page-scene.h"

#include <QPainter>

#include "gui-util/painter-saver.h"


namespace myapp {

PageScene::PageScene(Controller &controller, QObject *parent)
    : QObject(parent)
    , controller_(controller)
{
}

PageScene::~PageScene()
{
}

void PageScene::draw(QPainter &painter) const
{
    PainterSaver saver(painter);

    painter.translate(sceneSize_.width() / 2, sceneSize_.height() / 2);

    if (primaryPage_) {
        primaryPage_->draw(painter);
    }
}

void PageScene::setPrimaryPage(DeclarePtr<PageSprite> &&sprite)
{
    primaryPage_ = std::move(sprite);
}

void PageScene::updateSceneSize(const QSizeF &sceneSize)
{
    sceneSize_ = sceneSize;

    layoutPages();
}

void PageScene::rotatePagesByOneStep()
{
    if (primaryPage_) {
        primaryPage_->adjustArea(sceneSize_);
        primaryPage_->rotateByOneStep();
        layoutPage(*primaryPage_);
    }
}

void PageScene::layoutPages()
{
    if (primaryPage_) {
        layoutPage(*primaryPage_);
    }
}

void PageScene::layoutPage(PageSprite &sprite)
{
    sprite.adjustArea(sceneSize_);
    sprite.moveTo(QPointF(sceneSize_.width() / 2, sceneSize_.height() / 2));
}


}
