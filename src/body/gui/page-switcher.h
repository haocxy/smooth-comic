#pragma once

#include <unordered_map>

#include <QWidget>

#include "core/basetype.h"
#include "core/obj-handle.h"

#include "util/sigconns.h"
#include "util/qtobj-strand-entry.h"

#include "book/page-num.h"
#include "book/page-info.h"


namespace myapp {

class Book;

// 页面切换器
// 是页面的管理器，管理页面的显示逻辑和页面的生命周期
// 一个页面切换器对象对应多个页面
class PageSwitcher : public QWidget {
    Q_OBJECT
public:
    explicit PageSwitcher(Book &book, QWidget *parent = nullptr);

    virtual ~PageSwitcher();

private:
    void bookClosed();

    void pageLoaded(const PageInfo &page);

private:
    QtObjStrandEntry strandEntry_;

    Book &book_;

    SigConns sigConns_;

    std::unordered_map<PageNum, PageInfo> loadedPages_{ 987 };
    opt<PageNum> waitingPage_;

    StrongHandle<PageSwitcher> handle_;
};

}
