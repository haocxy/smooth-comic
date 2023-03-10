#include "page-scene.h"

#include <QDebug>
#include <QPainter>

#include "book/book.h"

#include "gui-util/painter-saver.h"

#include "controller/controller.h"

#include "page-sprite.h"


namespace myapp {

PageScene::PageScene(Controller &controller, QObject *parent)
    : QObject(parent)
    , controller_(controller)
    , handle_(*this)
{
    sigConns_ += controller_.book().sigBookClosed.connect([this, h = handle_.weak()](const fs::path &archiveFile) {
        h.apply([this] {
            strandEntry_.exec([this] {
                reset();
                });
            });
    });

    sigConns_ += controller_.book().sigPageLoaded.connect([this, h = handle_.weak()](const PageInfo &page) {
        h.apply([this, &page] {
            strandEntry_.exec([this, page] {
                pageLoaded(page);
                });
            });
    });

    using c = Controller;

    using s = PageScene;

    connect(&controller_, &c::cmdJumpToPage, this, &s::jumpTo);

    connect(&controller_, &c::cmdSwitchPage, this, &s::jumpBy);

    connect(&controller_, &c::cmdRotatePageByOneStep, this,
        &s::rotatePagesByOneStep);

    connect(&controller_, &c::cmdSetScaleMode, this, &s::setScaleMode);

    connect(&controller_, &c::cmdZoomIn, this, &s::zoomIn);

    connect(&controller_, &c::cmdZoomOut, this, &s::zoomOut);

    connect(&controller_, &c::cmdRelativelyScale, this, &s::relativelyScale);

    connect(&controller_, &c::cmdTranslateBy, this, &s::translateBy);
}

PageScene::~PageScene()
{
}

PageScene::MoveLock PageScene::determineMoveLock() const
{
    using Lock = PageScene::MoveLock;

    switch (scaleMode_) {
    case ScaleMode::RawSize:
    {
        const QSizeF realSize = primaryPage_->realSize();
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

static qreal goodPrimaryPageOffset(qreal ratioOffset, qreal pageLength, qreal sceneLength)
{
    if (pageLength <= sceneLength) {
        // ????????????????????????
        // ???????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????
        return sceneLength / 2;
    } else {
        return ratioOffset * sceneLength;
    }
}

QPointF PageScene::goodPrimaryPagePoint() const
{
    const qreal sw = sceneSize_.width();
    const qreal sh = sceneSize_.height();

    const qreal rx = primaryPageRatioPos_.x();
    const qreal ry = primaryPageRatioPos_.y();

    const QSizeF realSize = primaryPage_->realSize();

    const qreal goodX = goodPrimaryPageOffset(rx, realSize.width(), sceneSize_.width());
    const qreal goodY = goodPrimaryPageOffset(ry, realSize.height(), sceneSize_.height());

    return QPointF(goodX, goodY);
}

void PageScene::adjustSpritePos(PageSprite &sprite)
{
    if (sprite.isMovable(sceneSize_)) {

        QPointF targetPos = goodPrimaryPagePoint();

        // ???????????????????????????????????????????????????????????????
        QRectF tryRect = sprite.spriteRectForPos(targetPos);

        bool topMoved = false;
        if (tryRect.top() > 0) {
            const qreal dy = -tryRect.top();
            targetPos.ry() += dy;
            tryRect.translate(0, dy);
            topMoved = true;
        }
        if (tryRect.bottom() < sceneSize_.height()) {
            const qreal space = sceneSize_.height() - tryRect.bottom() - 1;
            if (topMoved) {
                targetPos.ry() += space / 2;
            } else {
                targetPos.ry() += space;
            }
        }

        bool leftMoved = false;
        if (tryRect.left() > 0) {
            const qreal dx = -tryRect.left();
            targetPos.rx() += dx;
            tryRect.translate(dx, 0);
            leftMoved = true;
        }
        if (tryRect.right() < sceneSize_.width()) {
            const qreal space = sceneSize_.width() - tryRect.right() - 1;
            if (leftMoved) {
                targetPos.rx() += space / 2;
            } else {
                targetPos.rx() += space;
            }
        }
        sprite.moveTo(targetPos);
    } else {
        sprite.moveTo(QPointF(sceneSize_.width() / 2, sceneSize_.height() / 2));
    }
}

void PageScene::savePrimaryPageRatioPos()
{
    const QPointF pos = primaryPage_->pos();
    primaryPageRatioPos_.setX(qreal(pos.x()) / sceneSize_.width());
    primaryPageRatioPos_.setY(qreal(pos.y()) / sceneSize_.height());
}

void PageScene::savePrimaryPageRatioSize()
{
    const QSizeF realSize = primaryPage_->realSize();
    primaryPageRatioSize_ = QSizeF(
        qreal(realSize.width()) / sceneSize_.width(),
        qreal(realSize.height()) / sceneSize_.height()
    );
}

void PageScene::savePrimaryPageRatioSize(bool recalcRatioWidth, bool recalcRatioHeight)
{
    if (!primaryPageRatioSize_) [[unlikely]] {
        savePrimaryPageRatioSize();
        return;
    }

    if (recalcRatioWidth || recalcRatioHeight) {

        const QSizeF realSize = primaryPage_->realSize();

        if (recalcRatioWidth) {
            primaryPageRatioSize_->setWidth(qreal(realSize.width()) / sceneSize_.width());
        }

        if (recalcRatioHeight) {
            primaryPageRatioSize_->setHeight(qreal(realSize.height()) / sceneSize_.height());
        }
    }
}

void PageScene::savePrimaryPageRatioWidth()
{
    const QSizeF realSize = primaryPage_->realSize();
    primaryPageRatioSize_->setWidth(qreal(realSize.width()) / sceneSize_.width());
}

void PageScene::savePrimaryPageRatioHeight()
{
    const QSizeF realSize = primaryPage_->realSize();
    primaryPageRatioSize_->setHeight(qreal(realSize.height()) / sceneSize_.height());
}

void PageScene::savePrimaryPagePixelSize()
{
    primaryPagePixelSize_ = primaryPage_->realSize();
}

void PageScene::setScaleMode(ScaleMode scaleMode)
{
    if (scaleMode_ != scaleMode) {
        scaleMode_ = scaleMode;
        emit scaleModeChanged();
        layoutPages();
    }
}

static qreal boundScale(qreal scale, qreal min, qreal max)
{
    if (scale < min) [[unlikely]] {
        return min;
    }

    if (scale > max) [[unlikely]] {
        return max;
    }

    return scale;
}

void PageScene::setScale(qreal scale)
{
    if (primaryPage_) {
        const qreal boundedScale = boundScale(scale, minScale_, maxScale_);
        if (primaryPage_->scale() != boundedScale) {
            primaryPage_->setScale(boundedScale);
            savePrimaryPagePixelSize();
            savePrimaryPageRatioSize();
            emit controller_.sigScaleUpdated(boundedScale);
            emit cmdUpdate();
        }
    }
}

void PageScene::relativelyScale(qreal relativeScale)
{
    if (primaryPage_) {
        setScale(primaryPage_->scale() * relativeScale);
    }
}

void PageScene::zoomIn()
{
    if (primaryPage_) {
        setScale(primaryPage_->scale() + 0.1);
    }
}

void PageScene::zoomOut()
{
    if (primaryPage_) {
        setScale(primaryPage_->scale() - 0.1);
    }
}

bool PageScene::isPageMovable() const
{
    return primaryPage_ && primaryPage_->isMovable(sceneSize_);
}

static qreal boundMoveVal(qreal x, qreal max)
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

void PageScene::movePage(qreal dx, qreal dy)
{
    if (primaryPage_) {
        const QRectF spriteRect = primaryPage_->spriteRect();

        const MoveLock lock = determineMoveLock();

        qreal realDX = 0;
        if (lock != MoveLock::LockHorizontal) {
            if (dx > 0) {
                realDX = boundMoveVal(dx, -spriteRect.left());
            } else if (dx < 0) {
                realDX = -boundMoveVal(-dx, spriteRect.right() - sceneSize_.width());
            }
        }

        qreal realDY = 0;
        if (lock != MoveLock::LocakVertical) {
            if (dy > 0) {
                realDY = boundMoveVal(dy, -spriteRect.top());
            } else if (dy < 0) {
                realDY = -boundMoveVal(-dy, spriteRect.bottom() - sceneSize_.height());
            }
        }

        primaryPage_->moveBy(realDX, realDY);

        savePrimaryPageRatioPos();

        emit cmdUpdate();
    }
}

void PageScene::jumpTo(PageNum pageNum)
{
    if (!loadedPages_.contains(pageNum)) {
        return;
    }

    // ???????????????????????????????????????????????????
    // ?????????????????????????????????????????????????????????
    if (loadingPage_) {
        return;
    }

    loadingPage_ = pageNum;

    controller_.book().loadPageImg(pageNum, [this, pageNum, h = handle_.weak()](const QPixmap &img) {
        h.apply([this, &pageNum, &img] {
            strandEntry_.exec([this, pageNum, img] {
                setPrimaryPage(new PageSprite(pageNum, img));
                loadingPage_ = std::nullopt;
            });
        });
    });
}

void PageScene::jumpNext()
{
    if (primaryPage_) {
        jumpTo(primaryPage_->pageSeq() + 1);
    }
}

void PageScene::jumpPrev()
{
    if (primaryPage_) {
        jumpTo(primaryPage_->pageSeq() - 1);
    }
}

void PageScene::jumpBy(SwitchDirection direction)
{
    switch (direction) {
    case SwitchDirection::Left:
        jumpNext();
        break;
    case SwitchDirection::Right:
        jumpPrev();
        break;
    case SwitchDirection::Up:
        jumpPrev();
        break;
    case SwitchDirection::Down:
        jumpNext();
        break;
    default:
        break;
    }
}

void PageScene::draw(QPainter &painter) const
{
    painter.setRenderHint(QPainter::RenderHint::SmoothPixmapTransform);

    PainterSaver saver(painter);

    painter.translate(sceneSize_.width() / 2, sceneSize_.height() / 2);

    if (primaryPage_) {
        primaryPage_->draw(painter);
    }
}

void PageScene::setPrimaryPage(DeclarePtr<PageSprite> &&sprite)
{
    primaryPage_ = std::move(sprite);

    updateScaleRange();

    layoutPages();

    emit cmdUpdate();
}

void PageScene::updateSceneSize(const QSizeF &sceneSize)
{
    sceneSize_ = sceneSize;

    updateScaleRange();

    layoutPages();
}

void PageScene::rotatePagesByOneStep()
{
    if (primaryPage_) {
        pageDir_ = nextOf(pageDir_);
        layoutPage(*primaryPage_);
    }
}

ScaleMode PageScene::scaleMode() const
{
    return scaleMode_;
}

void PageScene::reset()
{
    loadedPages_.clear();

    // TODO
}

void PageScene::pageLoaded(const PageInfo &page)
{
    loadedPages_[page.seqNum] = page;

    // TODO ????????? Book ?????????????????????????????????????????????

    if (0 == page.seqNum) {
        jumpTo(0);
    }
}

void PageScene::updateScaleRange()
{
    if (primaryPage_) {

        // ??????????????????????????????????????????????????????????????????????????????

        const qreal minScale = primaryPage_->calcMinScale(sceneSize_);
        if (minScale < minScale_) {
            minScale_ = minScale;
        }

        const qreal maxScale = primaryPage_->calcMaxScale(sceneSize_);
        if (maxScale > maxScale_) {
            maxScale_ = maxScale;
        }

        emit controller_.sigScaleRangeUpdated(minScale_, maxScale_);
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
    const qreal oldScale = sprite.scale();

    // ??????????????????
    sprite.rotateTo(pageDir_);

    // ??????????????????
    adjustPageSize(sprite);

    // ??????????????????
    adjustSpritePos(sprite);

    // ????????????????????????
    savePrimaryPageRatioPos();

    // ????????????????????????
    // ???????????????????????????????????????????????????????????????????????????????????????????????????????????????
    savePrimaryPageRatioSize(
        shouldRecalcRatioWidth(),
        shouldRecalcRatioHeight()
    );

    // ????????????????????????
    savePrimaryPagePixelSize();

    const qreal newScale = sprite.scale();
    if (oldScale != newScale) {
        emit controller_.sigScaleUpdated(newScale);
    }

    // ?????????????????????????????????
    emit cmdUpdate();
}

void PageScene::adjustPageSize(PageSprite &sprite)
{
    switch (scaleMode_) {
    case ScaleMode::RawSize:
        sprite.setScale(1.0);
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
        if (primaryPageRatioSize_) {
            sprite.adjustAreaWidth(sceneSize_.width() * primaryPageRatioSize_->width());
        } else {
            sprite.adjustAreaSize(sceneSize_);
        }
        break;
    case ScaleMode::FixHeightByRatio:
        if (primaryPageRatioSize_) {
            sprite.adjustAreaHeight(sceneSize_.height() * primaryPageRatioSize_->height());
        } else {
            sprite.adjustAreaSize(sceneSize_);
        }
        break;
    case ScaleMode::FixWidthByPixel:
        if (primaryPagePixelSize_) {
            sprite.adjustAreaWidth(primaryPagePixelSize_->width());
        } else {
            sprite.adjustAreaSize(sceneSize_);
        }
        break;
    case ScaleMode::FixHeightByPixel:
        if (primaryPagePixelSize_) {
            sprite.adjustAreaHeight(primaryPagePixelSize_->height());
        } else {
            sprite.adjustAreaSize(sceneSize_);
        }
        break;
    default:
        break;
    }
}

bool PageScene::shouldRecalcRatioWidth() const
{
    switch (scaleMode_) {
    case ScaleMode::FixWidthByRatio:
        return false;
    default:
        return true;
    }
}

bool PageScene::shouldRecalcRatioHeight() const
{
    switch (scaleMode_) {
    case ScaleMode::FixHeightByRatio:
        return false;
    default:
        return true;
    }
}


}
