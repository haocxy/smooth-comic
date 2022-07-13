#pragma once

#include <map>
#include <optional>

#include <QObject>
#include <QSize>

#include "core/declare_ptr.h"
#include "core/obj-handle.h"

#include "book/page-num.h"

#include "gui/gui-util/qtobj-strand-entry.h"

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

    void updateSceneSize(const QSizeF &sceneSize);

    void rotatePagesByOneStep();

signals:
    void cmdUpdate();

    void sigPrimaryPagePrepared();

private:
    void layoutPages();

    void layoutPage(PageSprite &sprite);

    void setPrimaryPage(DeclarePtr<PageSprite> &&sprite);

    void preparePrimaryPage(PageNum seqNum);

private:
    QtObjStrandEntry strandEntry_;

    Controller &controller_;

    PageNum primaryPageSeq_{ 0 };

    DeclarePtr<PageSprite> primaryPage_;

    QSizeF sceneSize_;


    StrongHandle<PageScene> handle_;
};

}
