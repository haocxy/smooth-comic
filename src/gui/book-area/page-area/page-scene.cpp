#include "page-scene.h"

#include <QPainter>

#include "gui-util/painter-saver.h"


namespace myapp {

PageScene::PageScene(QObject *parent)
    : QObject(parent)
{
}

PageScene::~PageScene()
{
}

void PageScene::draw(QPainter &painter) const
{
    PainterSaver saver(painter);

    painter.translate(sceneSize_.width() / 2, sceneSize_.height() / 2);

    for (const auto &pair : sprites_) {
        const PageNum seqNum = pair.first;
        const DeclarePtr<PageSprite> &sprite = pair.second;

        sprite->draw(painter);
    }
}

void PageScene::addPage(PageNum seqNum, DeclarePtr<PageSprite> &&sprite)
{
    DeclarePtr<PageSprite> &p = sprites_[seqNum];

    p = std::move(sprite);
}

void PageScene::updateSceneSize(const QSizeF &sceneSize)
{
    sceneSize_ = sceneSize;

    layoutPages();
}

void PageScene::rotatePagesByOneStep()
{
    for (const auto &pair : sprites_) {
        const DeclarePtr<PageSprite> &sprite = pair.second;
        sprite->adjustArea(sceneSize_);
        sprite->rotateByOneStep();
        layoutPage(*sprite);
    }
}

void PageScene::layoutPages()
{
    for (const auto &pair : sprites_) {
        const DeclarePtr<PageSprite> &sprite = pair.second;
        layoutPage(*sprite);
    }
}

void PageScene::layoutPage(PageSprite &sprite)
{
    sprite.adjustArea(sceneSize_);
    sprite.moveTo(QPointF(sceneSize_.width() / 2, sceneSize_.height() / 2));
}


}
