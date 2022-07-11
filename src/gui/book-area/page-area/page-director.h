#pragma once

#include <unordered_map>

#include <QObject>
#include <QSize>

#include "core/basetype.h"
#include "core/obj-handle.h"
#include "core/declare_ptr.h"
#include "util/sigconns.h"

#include "gui-util/qtobj-strand-entry.h"

#include "controller/switch-direction.h"

#include "book/page-info.h"

#include "page-direction.h"


class QPainter;


namespace myapp {

class Controller;

class PageScene;


class PageDirector : public QObject {
    Q_OBJECT
public:
    explicit PageDirector(Controller &controller, QObject *parent = nullptr);

    virtual ~PageDirector();

    void updateShowSize(const QSize &size);

    void draw(QPainter &painter) const;

    // 跳转到指定页，只能是已经加载的页
    void jumpTo(PageNum pageNum);

    //
    void jumpNext();

    void jumpPrev();

    void switchNextPage(SwitchDirection direction);

    void rotatePageByOneStep();

signals:
    void cmdUpdate();

private:
    void reset();

    void pageLoaded(const PageInfo &page);

    void asyncLoadImg(PageNum seqNum);

    void onLoadPageImgDone(PageNum seqNum, const QPixmap &img);

private:
    Controller &controller_;

    QtObjStrandEntry strandEntry_;

    SigConns sigConns_;

    std::unordered_map<PageNum, PageInfo> loadedPages_{ 987 };

    opt<PageNum> waitingPage_;

    opt<PageNum> currPage_;

    QSize showSize_;

    DeclarePtr<PageScene> primaryScene_;

    StrongHandle<PageDirector> handle_;
};

}
