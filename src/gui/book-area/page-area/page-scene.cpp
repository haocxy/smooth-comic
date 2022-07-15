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

static int goodPrimaryPageOffset(qreal ratioOffset, int pageLength, int sceneLength)
{
    if (pageLength <= sceneLength) {
        return sceneLength / 2;
    } else {
        return int(std::round(ratioOffset * sceneLength));
    }
}

QPoint PageScene::goodPrimaryPagePoint() const
{
    const int sw = sceneSize_.width();
    const int sh = sceneSize_.height();

    const qreal rx = primaryPageRatioPos_.x();
    const qreal ry = primaryPageRatioPos_.y();

    const QSize realSize = primaryPage_->realSize();

    const int goodX = goodPrimaryPageOffset(rx, realSize.width(), sceneSize_.width());
    const int goodY = goodPrimaryPageOffset(ry, realSize.height(), sceneSize_.height());

    return QPoint(goodX, goodY);
}

void PageScene::adjustSpritePos(PageSprite &sprite)
{
    if (sprite.isMovable(sceneSize_)) {

        QPoint targetPos = goodPrimaryPagePoint();

        // 如果四周有多余的空间，则调整页面位置以利用
        QRect tryRect = sprite.spriteRectForPos(targetPos);

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

void PageScene::savePrimaryPageRatioPos()
{
    const QPoint pos = primaryPage_->pos();
    primaryPageRatioPos_.setX(qreal(pos.x()) / sceneSize_.width());
    primaryPageRatioPos_.setY(qreal(pos.y()) / sceneSize_.height());
}

void PageScene::savePrimaryPageRatioX()
{
    const QPoint pos = primaryPage_->pos();
    primaryPageRatioPos_.setX(qreal(pos.x()) / sceneSize_.width());
}

void PageScene::savePrimaryPageRatioY()
{
    const QPoint pos = primaryPage_->pos();
    primaryPageRatioPos_.setY(qreal(pos.y()) / sceneSize_.height());
}

void PageScene::savePrimaryPageRatioSize()
{
    const QSize realSize = primaryPage_->realSize();
    primaryPageRatioSize_ = QSizeF(
        qreal(realSize.width()) / sceneSize_.width(),
        qreal(realSize.height()) / sceneSize_.height()
    );
}

void PageScene::savePrimaryPageRatioWidth()
{
    const QSize realSize = primaryPage_->realSize();
    primaryPageRatioSize_->setWidth(qreal(realSize.width()) / sceneSize_.width());
}

void PageScene::savePrimaryPageRatioHeight()
{
    const QSize realSize = primaryPage_->realSize();
    primaryPageRatioSize_->setHeight(qreal(realSize.height()) / sceneSize_.height());
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

        switch (scaleMode_) {
        case ScaleMode::NoScale:
        case ScaleMode::AutoFitAreaSize:
            savePrimaryPageRatioPos();
            break;
        case ScaleMode::AutoFitAreaWidth:
        case ScaleMode::FixWidthByRatio:
        case ScaleMode::FixWidthByPixel:
            savePrimaryPageRatioY();
            break;
        case ScaleMode::AutoFitAreaHeight:
        case ScaleMode::FixHeightByRatio:
        case ScaleMode::FixHeightByPixel:
            savePrimaryPageRatioX();
            break;
        default:
            break;
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
        savePrimaryPageRatioPos();
        savePrimaryPageRatioSize();
        break;
    case ScaleMode::AutoFitAreaSize:
        sprite.adjustAreaSize(sceneSize_);
        savePrimaryPageRatioPos();
        savePrimaryPageRatioSize();
        break;
    case ScaleMode::AutoFitAreaWidth:
        sprite.adjustAreaWidth(sceneSize_.width());
        savePrimaryPageRatioPos();
        savePrimaryPageRatioSize();
        break;
    case ScaleMode::AutoFitAreaHeight:
        sprite.adjustAreaHeight(sceneSize_.height());
        savePrimaryPageRatioPos();
        savePrimaryPageRatioSize();
        break;
    case ScaleMode::FixWidthByRatio:
        if (primaryPageRatioSize_) {
            sprite.adjustAreaWidth(std::round(sceneSize_.width() * primaryPageRatioSize_->width()));
        } else {
            sprite.adjustAreaSize(sceneSize_);
        }
        savePrimaryPageRatioY();
        savePrimaryPageRatioHeight();
        break;
    case ScaleMode::FixHeightByRatio:
        if (primaryPageRatioSize_) {
            sprite.adjustAreaHeight(std::round(sceneSize_.height() * primaryPageRatioSize_->height()));
        } else {
            sprite.adjustAreaSize(sceneSize_);
        }
        savePrimaryPageRatioX();
        savePrimaryPageRatioWidth();
        break;
    default:
        break;
    }

    adjustSpritePos(sprite);

    emit cmdUpdate();
}


}
