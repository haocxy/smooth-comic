#pragma once

#include <map>

#include <QObject>

#include "core/declare_ptr.h"

#include "book/page-num.h"

#include "page-sprite.h"


namespace myapp {

class PageScene : public QObject {
    Q_OBJECT
public:
    explicit PageScene(QObject *parent = nullptr);

    virtual ~PageScene();

    void addPage(PageNum seqNum, DeclarePtr<PageSprite> &&sprite);

private:
    std::map<PageNum, DeclarePtr<PageSprite>> sprites_;
};

}
