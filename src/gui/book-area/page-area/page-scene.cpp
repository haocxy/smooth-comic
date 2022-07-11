#include "page-scene.h"


namespace myapp {

PageScene::PageScene(QObject *parent)
    : QObject(parent)
{
}

PageScene::~PageScene()
{
}

void PageScene::addPage(PageNum seqNum, DeclarePtr<PageSprite> &&sprite)
{
    sprites_[seqNum] = std::move(sprite);
}

}
