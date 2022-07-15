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
    using Lock = PageScene::MoveLock;

    switch (scaleMode_) {
    case ScaleMode::NoScale:
    {
        const QSize realSize = primaryPage_->realSize();
        const bool hOver = realSize.width() > sceneSize_.width();
        const bool vOver = realSize.height() > sceneSize_.height();
        if (hOver && vOver) {
            return Lock::NoLock;
        } else if (hOver) {
            return Lock::LocakVertical;
        } else {
            // if vOver
            return Lock::LockHorizontal;
        }
    }
    case ScaleMode::AutoFitAreaWidth:
        return Lock::LockHorizontal;
    case ScaleMode::AutoFitAreaHeight:
        return Lock::LocakVertical;
    default:
        return Lock::NoLock;
    }
}

void PageScene::adjustSpritePos(PageSprite &sprite)
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
            const QPointF ratioPos = sprite.ratioPos();
            const QSize spriteSize = sprite.realSize();
            if (spriteSize.width() > sceneSize_.width() && spriteSize.height() > sceneSize_.height()) {
                targetPos.setX(std::round(sceneSize_.width() * ratioPos.x()));
                targetPos.setY(std::round(sceneSize_.height() * ratioPos.y()));
            } else if (spriteSize.width() > sceneSize_.width()) {
                targetPos.setX(std::round(sceneSize_.width() * ratioPos.x()));
                targetPos.setY(sprite.pos().y());
            } else if (spriteSize.height() > sceneSize_.height()) {
                targetPos.setX(sprite.pos().x());
                targetPos.setY(std::round(sceneSize_.height() * ratioPos.y()));
            }
        }

        // 如果四周有多余的空间，则调整页面位置以利用
        QRect tryRect = sprite.spriteRectForPos(targetPos);

        //const int hUsableSpace = sceneSize_.width() - tryRect.width();
        //const int vUsableSpace = sceneSize_.height() - tryRect.height();

        //if (hUsableSpace > 0) {
        //    const int leftUsableSpace = tryRect.left();
        //    const int rightUsableSpace = sceneSize_.width() - tryRect.right() - 1;
        //    if (leftUsableSpace >= rightUsableSpace) {
        //        targetPos.rx() -= hUsableSpace / 2;
        //    } else {
        //        targetPos.rx() += hUsableSpace / 2;
        //    }
        //}

        //if (vUsableSpace > 0) {
        //    const int topUsableSpace = tryRect.top();
        //    const int bottomUsableSpace = sceneSize_.height() - tryRect.bottom() - 1;
        //    if (topUsableSpace > 0 && bottomUsableSpace <= 0) {
        //        targetPos.ry() -= vUsableSpace / 2;
        //    } else if (bottomUsableSpace > 0 && topUsableSpace <= 0) {
        //        targetPos.ry() += vUsableSpace / 2;
        //    }
        //}

        bool topMoved = false;
        if (tryRect.top() > 0) {
            const int dy = -tryRect.top();
            targetPos.ry() += dy;
            tryRect.translate(0, dy);
            topMoved = true;
        }
        if (tryRect.bottom() < sceneSize_.height()) {
            const int space = sceneSize_.height() - tryRect.bottom() - 1;
            if (topMoved) {
                targetPos.ry() += space / 2;
            } else {
                targetPos.ry() += space;
            }
        }

        bool leftMoved = false;
        if (tryRect.left() > 0) {
            const int dx = -tryRect.left();
            targetPos.rx() += dx;
            tryRect.translate(dx, 0);
            leftMoved = true;
        }
        if (tryRect.right() < sceneSize_.width()) {
            const int space = sceneSize_.width() - tryRect.right() - 1;
            if (leftMoved) {
                targetPos.rx() += space / 2;
            } else {
                targetPos.rx() += space;
            }
        }
        sprite.moveTo(targetPos);
    } else {
        sprite.moveTo(QPoint(sceneSize_.width() / 2, sceneSize_.height() / 2));
    }
}

bool PageScene::shouldUpdateRatio() const
{
    // 在某些缩放模式中不更新比例，避免因浮点数误差导致的多次调整窗口尺寸后积累误差

    switch (scaleMode_) {
    case ScaleMode::FixWidthByRatio:
    case ScaleMode::FixHeightByRatio:
        return false;
    default:
        return true;
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

    if (max < 0) [[unlikely]] {
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

        if (shouldUpdateRatio()) {
            primaryPage_->setRatioPosByAreaSize(sceneSize_);
        }

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
    case ScaleMode::NoScale:
        sprite.scale(1.0);
        break;
    case ScaleMode::AutoFitAreaSize:
        sprite.adjustAreaSize(sceneSize_);
        break;
    case ScaleMode::AutoFitAreaWidth:
        sprite.adjustAreaWidth(sceneSize_.width());
        break;
    case ScaleMode::AutoFitAreaHeight:
        sprite.adjustAreaHeight(sceneSize_.height());
        break;
    case ScaleMode::FixWidthByRatio:
        if (sprite.ratioSize()) {
            sprite.adjustAreaWidth(std::round(sceneSize_.width() * sprite.ratioSize()->x()));
        } else {
            sprite.adjustAreaSize(sceneSize_);
        }
        break;
    case ScaleMode::FixHeightByRatio:
        if (sprite.ratioSize()) {
            sprite.adjustAreaHeight(std::round(sceneSize_.height() * sprite.ratioSize()->y()));
        } else {
            sprite.adjustAreaSize(sceneSize_);
        }
        break;
    default:
        break;
    }

    adjustSpritePos(sprite);

    if (shouldUpdateRatio()) {
        sprite.setRatioPosByAreaSize(sceneSize_);
        sprite.setRatioSizeByAreaSize(sceneSize_);
    }

    emit cmdUpdate();
}


}
