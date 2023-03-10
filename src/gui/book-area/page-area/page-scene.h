#pragma once

#include <map>

#include <optional>

#include <QObject>
#include <QSize>
#include <QPoint>

#include "core/basetype.h"
#include "core/declare_ptr.h"
#include "core/obj-handle.h"

#include "util/sigconns.h"

#include "book/page-num.h"
#include "book/page-info.h"

#include "gui/gui-util/qtobj-strand-entry.h"

#include "controller/scale-mode.h"
#include "controller/switch-direction.h"

#include "page-direction.h"


class QPainter;


namespace myapp {

class Controller;

class PageSprite;


class PageScene : public QObject {
    Q_OBJECT
public:
    explicit PageScene(Controller &controller, QObject *parent = nullptr);

    virtual ~PageScene();

    void draw(QPainter &painter) const;

    void updateSceneSize(const QSizeF &sceneSize);

    void rotatePagesByOneStep();

    ScaleMode scaleMode() const;

    void setScaleMode(ScaleMode scaleMode);

    void setScale(qreal scale);

    void relativelyScale(qreal relativeScale);

    void zoomIn();

    void zoomOut();

    bool isPageMovable() const;

    void movePage(qreal dx, qreal dy);

    void translateBy(qreal dx, qreal dy) {
        movePage(dx, dy);
    }

    void jumpTo(PageNum pageNum);

    void jumpNext();

    void jumpPrev();

    void jumpBy(SwitchDirection direction);

signals:
    void cmdUpdate();

    void scaleModeChanged();

private:
    void reset();

    void pageLoaded(const PageInfo &page);

    void updateScaleRange();

    void layoutPages();

    void layoutPage(PageSprite &sprite);

    void adjustPageSize(PageSprite &sprite);

    bool shouldRecalcRatioWidth() const;

    bool shouldRecalcRatioHeight() const;

    void setPrimaryPage(DeclarePtr<PageSprite> &&sprite);

    enum class MoveLock {
        NoLock, LockHorizontal, LocakVertical,
    };

    MoveLock determineMoveLock() const;

    QPointF goodPrimaryPagePoint() const;

    void adjustSpritePos(PageSprite &sprite);

    void savePrimaryPageRatioPos();

    void savePrimaryPageRatioSize();

    void savePrimaryPageRatioSize(bool recalcRatioWidth, bool recalcRatioHeight);

    void savePrimaryPageRatioWidth();

    void savePrimaryPageRatioHeight();

    void savePrimaryPagePixelSize();

private:
    QtObjStrandEntry strandEntry_;

    SigConns sigConns_;

    Controller &controller_;

    std::map<PageNum, PageInfo> loadedPages_;

    std::optional<PageNum> loadingPage_;

    DeclarePtr<PageSprite> primaryPage_;

    QSizeF sceneSize_;

    PageDirection pageDir_{ PageDirection::Up };

    ScaleMode scaleMode_{ ScaleMode::AutoFitAreaSize };

    // ?????????????????????????????????????????????????????????????????????????????????
    qreal minScale_{ std::numeric_limits<qreal>::max() };

    // ?????????????????????????????????????????????????????????????????????????????????
    qreal maxScale_{ std::numeric_limits<qreal>::min() };

    // ????????????????????????????????????????????????????????????????????????
    QPointF primaryPageRatioPos_{ 0, 0 };

    // ??????????????????????????????????????????????????????????????????
    std::optional<QSizeF> primaryPageRatioSize_;

    std::optional<QSizeF> primaryPagePixelSize_;

    StrongHandle<PageScene> handle_;
};

}
