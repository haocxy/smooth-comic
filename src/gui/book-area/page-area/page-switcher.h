#pragma once

#include <unordered_map>

#include <QWidget>
#include <QPointer>
#include <QBoxLayout>

#include "core/basetype.h"
#include "core/obj-handle.h"

#include "util/sigconns.h"
#include "gui-util/qtobj-strand-entry.h"

#include "book/page-num.h"
#include "book/page-info.h"

#include "switch-direction.h"


namespace myapp {

class Book;

class PageWidget;

// 页面切换器
// 是页面的管理器，管理页面的显示逻辑和页面的生命周期
// 一个页面切换器对象对应多个页面
class PageSwitcher : public QWidget {
    Q_OBJECT
public:
    explicit PageSwitcher(Book &book, QWidget *parent = nullptr);

    virtual ~PageSwitcher();

    // 跳转到指定页，只能是已经加载的页
    void jumpTo(PageNum pageNum);

    //
    void jumpNext();

    void jumpPrev();

    void switchNextPage(SwitchDirection direction);

private:
    void reset();

    void pageLoaded(const PageInfo &page);

    void asyncLoadImg(PageNum seqNum);

    void onLoadPageImgDone(PageNum seqNum, const QPixmap &img);

private:
    QtObjStrandEntry strandEntry_;

    Book &book_;

    SigConns sigConns_;

    std::unordered_map<PageNum, PageInfo> loadedPages_{ 987 };

    opt<PageNum> waitingPage_;

    QPointer<QLayout> pageLayout_;
    QPointer<PageWidget> pageWidget_;

    StrongHandle<PageSwitcher> handle_;
};

}
