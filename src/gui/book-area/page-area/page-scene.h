#pragma once

#include <map>

#include <QObject>
#include <QSize>

#include "core/declare_ptr.h"

#include "book/page-num.h"

#include "page-sprite.h"


class QPainter;


namespace myapp {

class Controller;


class PageScene : public QObject {
    Q_OBJECT
public:
    explicit PageScene(Controller &controller, QObject *parent = nullptr);

    virtual ~PageScene();

    void draw(QPainter &painter) const;

    void setPrimaryPage(DeclarePtr<PageSprite> &&sprite);

    void updateSceneSize(const QSizeF &sceneSize);

    void rotatePagesByOneStep();

private:
    void layoutPages();

    void layoutPage(PageSprite &sprite);

private:
    Controller &controller_;

private:
    DeclarePtr<PageSprite> primaryPage_;

    QSizeF sceneSize_;
};

}
