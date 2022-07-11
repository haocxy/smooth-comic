#include "page-sprite.h"


namespace myapp {

PageSprite::PageSprite(const QPixmap &rawImg, QObject *parent)
    : QObject(parent)
    , rawImg_(rawImg)
{
}

PageSprite::~PageSprite()
{
}

}
