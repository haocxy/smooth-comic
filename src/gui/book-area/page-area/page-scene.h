#pragma once

#include <map>

#include <QObject>
#include <QSize>

#include "core/declare_ptr.h"

#include "book/page-num.h"

#include "page-sprite.h"


class QPainter;


namespace myapp {



class PageScene : public QObject {
    Q_OBJECT
public:
    explicit PageScene(QObject *parent = nullptr);

    virtual ~PageScene();

    void draw(QPainter &painter) const;

    void addPage(PageNum seqNum, DeclarePtr<PageSprite> &&sprite);

    void updateSceneSize(const QSizeF &sceneSize);

    void rotatePagesByOneStep();

private:
    void layoutPages();

    void layoutPage(PageSprite &sprite);

private:
    std::map<PageNum, DeclarePtr<PageSprite>> sprites_;

    QSizeF sceneSize_;
};

}
