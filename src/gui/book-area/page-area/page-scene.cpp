#include "page-scene.h"


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
    for (const auto &pair : sprites_) {
        const PageNum seqNum = pair.first;
        const DeclarePtr<PageSprite> &sprite = pair.second;
        sprite->draw(painter);
    }
}

void PageScene::addPage(PageNum seqNum, DeclarePtr<PageSprite> &&sprite)
{
    sprites_[seqNum] = std::move(sprite);
}

}
