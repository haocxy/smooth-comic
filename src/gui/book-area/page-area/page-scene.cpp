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

    connect(&controller_, &Controller::cmdSetScaleMode, this, &PageScene::setScaleMode);
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

void PageScene::onBecomePrimaryScene()
{
    assert(primaryPage_);

    const float minScale = primaryPage_->calcMinScale(sceneSize_);
    const float maxScale = primaryPage_->calcMaxScale(sceneSize_);

    emit controller_.sigScaleRangeUpdated(minScale, maxScale);
}

PageScene::MoveLock PageScene::determineMoveLock() const
{
    switch (scaleMode_) {
    case ScaleMode::AutoFitAreaWidth:
        return PageScene::MoveLock::LockHorizontal;
    case ScaleMode::AutoFitAreaHeight:
        return PageScene::MoveLock::LocakVertical;
    default:
        return PageScene::MoveLock::NoLock;
    }
}

void PageScene::adjustSpritePosByRatio(PageSprite &sprite)
{
    if (sprite.isMovable(sceneSize_)) {

        QPoint targetPos;
        if (scaleMode_ == ScaleMode::AutoFitAreaWidth) {
            targetPos.setX(sceneSize_.width() / 2);
            targetPos.setY(sceneSize_.height() * sprite.ratioPos().y());
        } else if (scaleMode_ == ScaleMode::AutoFitAreaHeight) {
            targetPos.setX(sceneSize_.width() * sprite.ratioPos().x());
            targetPos.setY(sceneSize_.height() / 2);
        } else {
            throw std::logic_error("PageScene::adjustSpritePosByRatio() bad scaleMode_");
        }

        const QRect tryRect = sprite.spriteRectForPos(targetPos);

        // 如果四周有多余的空间，则调整页面位置以利用
        if (tryRect.top() > 0) {
            targetPos.ry() -= tryRect.top();
        }
        if (tryRect.bottom() < sceneSize_.height()) {
            targetPos.ry() += sceneSize_.height() - tryRect.bottom();
        }
        if (tryRect.left() > 0) {
            targetPos.rx() -= tryRect.left();
        }
        if (tryRect.right() < sceneSize_.width()) {
            targetPos.rx() += sceneSize_.width() - tryRect.right();
        }
        sprite.moveTo(targetPos);
    } else {
        sprite.moveTo(QPoint(sceneSize_.width() / 2, sceneSize_.height() / 2));
    }
}

void PageScene::setScaleMode(ScaleMode scaleMode)
{
    if (scaleMode_ != scaleMode) {
        scaleMode_ = scaleMode;

        layoutPages();
    }
}

bool PageScene::isPageMovable() const
{
    return primaryPage_ && primaryPage_->isMovable(sceneSize_);
}

static int boundMoveVal(int x, int max)
{
    if (x < 0) [[unlikely]] {
        return 0;
    }

    if (x > max) [[unlikely]] {
        return max;
    }

    return x;
}

void PageScene::movePage(int dx, int dy)
{
    if (primaryPage_) {
        const QRect spriteRect = primaryPage_->spriteRect();

        const MoveLock lock = determineMoveLock();

        int realDX = 0;
        if (lock != MoveLock::LockHorizontal) {
            if (dx > 0) {
                realDX = boundMoveVal(dx, -spriteRect.left());
            } else if (dx < 0) {
                realDX = -boundMoveVal(-dx, spriteRect.right() - sceneSize_.width());
            }
        }

        int realDY = 0;
        if (lock != MoveLock::LocakVertical) {
            if (dy > 0) {
                realDY = boundMoveVal(dy, -spriteRect.top());
            } else if (dy < 0) {
                realDY = -boundMoveVal(-dy, spriteRect.bottom() - sceneSize_.height());
            }
        }

        primaryPage_->moveBy(realDX, realDY);

        primaryPage_->setRatioPosByAreaSize(sceneSize_);

        emit cmdUpdate();
    }
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

void PageScene::updateSceneSize(const QSize &sceneSize)
{
    sceneSize_ = sceneSize;

    layoutPages();
}

void PageScene::rotatePagesByOneStep()
{
    if (primaryPage_) {
        primaryPage_->rotateByOneStep();
        layoutPage(*primaryPage_);
    }
}

bool PageScene::isPrimaryScene() const
{
    return isPrimaryScene_;
}

void PageScene::setIsPrimaryScene(bool isPrimaryScene)
{
    if (isPrimaryScene_ != isPrimaryScene) {
        isPrimaryScene_ = isPrimaryScene;

        if (isPrimaryScene_ && primaryPage_) {
            onBecomePrimaryScene();
        }
    }
}

ScaleMode PageScene::scaleMode() const
{
    return scaleMode_;
}

void PageScene::layoutPages()
{
    if (primaryPage_) {
        layoutPage(*primaryPage_);
    }
}

void PageScene::layoutPage(PageSprite &sprite)
{
    switch (scaleMode_) {
    case ScaleMode::AutoFitAreaSize:
        sprite.adjustAreaSize(sceneSize_);
        sprite.moveTo(QPoint(sceneSize_.width() / 2, sceneSize_.height() / 2));
        break;
    case ScaleMode::AutoFitAreaWidth:
        sprite.adjustAreaWidth(sceneSize_.width());
        adjustSpritePosByRatio(sprite);
        break;
    case ScaleMode::AutoFitAreaHeight:
        sprite.adjustAreaHeight(sceneSize_.height());
        adjustSpritePosByRatio(sprite);
        break;
    case ScaleMode::Manual:
        // 手动定位模式时，不自动调整页面缩放
        break;
    default:
        break;
    }

    sprite.setRatioPosByAreaSize(sceneSize_);

    emit cmdUpdate();
}


}
