#pragma once

#include <map>
#include <optional>

#include <QObject>
#include <QSize>
#include <QPoint>

#include "core/basetype.h"
#include "core/declare_ptr.h"
#include "core/obj-handle.h"

#include "book/page-num.h"

#include "gui/gui-util/qtobj-strand-entry.h"

#include "controller/scale-mode.h"

#include "page-sprite.h"


class QPainter;


namespace myapp {

class Controller;


class PageScene : public QObject {
    Q_OBJECT
public:
    explicit PageScene(Controller &controller, PageNum primaryPage, QObject *parent = nullptr);

    virtual ~PageScene();

    void draw(QPainter &painter) const;

    PageNum primaryPageSeq() const;

    void updateSceneSize(const QSize &sceneSize);

    void rotatePagesByOneStep();

    bool isPrimaryScene() const;

    void setIsPrimaryScene(bool isPrimaryScene);

    ScaleMode scaleMode() const;

    void setScaleMode(ScaleMode scaleMode);

    bool isPageMovable() const;

    void movePage(int dx, int dy);

signals:
    void cmdUpdate();

    void sigPrimaryPagePrepared();

private:
    void layoutPages();

    void layoutPage(PageSprite &sprite);

    void setPrimaryPage(DeclarePtr<PageSprite> &&sprite);

    void preparePrimaryPage(PageNum seqNum);

    void onBecomePrimaryScene();

    enum class MoveLock {
        NoLock, LockHorizontal, LocakVertical,
    };

    MoveLock determineMoveLock() const;

    QPoint goodPrimaryPagePoint() const;

    void adjustSpritePos(PageSprite &sprite);

    void savePrimaryPageRatioPos();

    void savePrimaryPageRatioX();

    void savePrimaryPageRatioY();

    void savePrimaryPageRatioSize();

    void savePrimaryPageRatioWidth();

    void savePrimaryPageRatioHeight();

private:
    QtObjStrandEntry strandEntry_;

    Controller &controller_;

    bool isPrimaryScene_{ false };

    PageNum primaryPageSeq_{ 0 };

    DeclarePtr<PageSprite> primaryPage_;

    QSize sceneSize_;

    ScaleMode scaleMode_{ ScaleMode::AutoFitAreaSize };

    // 主页面锚点在显示区域的比例坐标，仅由外部逻辑使用
    QPointF primaryPageRatioPos_{ 0, 0 };

    // 主页面按比例计算的页面尺寸，仅由外部逻辑使用
    std::optional<QSizeF> primaryPageRatioSize_;

    StrongHandle<PageScene> handle_;
};

}
