#include "page-scene.h"

#include <QPainter>

#include "book/book.h"

#include "gui-util/painter-saver.h"

#include "controller/controller.h"


namespace myapp {

PageScene::PageScene(Controller &controller, PageNum primaryPage, QObject *parent)
    : QObject(parent)
    , controller_(controller)
    , primaryPageSeq_(primaryPage)
    , handle_(*this)
{
    preparePrimaryPage(primaryPageSeq_);
}

PageScene::~PageScene()
{
}

void PageScene::preparePrimaryPage(PageNum seqNum)
{
    controller_.book().loadPageImg(seqNum, [this, seqNum, h = handle_.weak()](const QPixmap &img) {
        h.apply([this, &seqNum, &img] {
            strandEntry_.exec([this, seqNum, img] {
                setPrimaryPage(new PageSprite(seqNum, img));
            });
        });
    });
}

void PageScene::draw(QPainter &painter) const
{
    PainterSaver saver(painter);

    painter.translate(sceneSize_.width() / 2, sceneSize_.height() / 2);

    if (primaryPage_) {
        primaryPage_->draw(painter);
    }
}

PageNum PageScene::primaryPageSeq() const
{
    return primaryPageSeq_;
}

void PageScene::setPrimaryPage(DeclarePtr<PageSprite> &&sprite)
{
    primaryPage_ = std::move(sprite);

    emit sigPrimaryPagePrepared();

    emit cmdUpdate();
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
