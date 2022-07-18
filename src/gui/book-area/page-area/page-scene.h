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

    void updateSceneSize(const QSize &sceneSize);

    void rotatePagesByOneStep();

    bool isPrimaryScene() const;

    void setIsPrimaryScene(bool isPrimaryScene);

    ScaleMode scaleMode() const;

    void setScaleMode(ScaleMode scaleMode);

    void setScale(float scale);

    void zoomIn();

    void zoomOut();

    bool isPageMovable() const;

    void movePage(int dx, int dy);

    void jumpTo(PageNum pageNum);

    void jumpNext();

    void jumpPrev();

    void jumpBy(SwitchDirection direction);

signals:
    void cmdUpdate();

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

    void onBecomePrimaryScene();

    enum class MoveLock {
        NoLock, LockHorizontal, LocakVertical,
    };

    MoveLock determineMoveLock() const;

    QPoint goodPrimaryPagePoint() const;

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

    bool isPrimaryScene_{ false };

    DeclarePtr<PageSprite> primaryPage_;

    QSize sceneSize_;

    ScaleMode scaleMode_{ ScaleMode::AutoFitAreaSize };

    // 初始值选择一个比所有可能的值都大的值以保证能够被初始化
    float minScale_{ std::numeric_limits<float>::max() };

    // 初始值选择一个比所有可能的值都小的值以保证能够被初始化
    float maxScale_{ std::numeric_limits<float>::min() };

    // 主页面锚点在显示区域的比例坐标，仅由外部逻辑使用
    QPointF primaryPageRatioPos_{ 0, 0 };

    // 主页面按比例计算的页面尺寸，仅由外部逻辑使用
    std::optional<QSizeF> primaryPageRatioSize_;

    std::optional<QSize> primaryPagePixelSize_;

    StrongHandle<PageScene> handle_;
};

}
