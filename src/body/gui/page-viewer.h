#pragma once

#include <QSplitter>
#include <QPointer>
#include <QStackedLayout>

#include "book/page-num.h"


namespace myapp {

class Book;

class PageSwitcher;


// 页面展示器
// 是页面展示区域的最外层组件
// 负责整合管理和页面显示、操作等有关的各GUI组件
class PageViewer : public QSplitter {
    Q_OBJECT
public:
    explicit PageViewer(Book &book, QWidget *parent = nullptr);

    void jumpTo(PageNum seqNum);

    void jumpNext();

    void jumpPrev();

private:
    Book &book_;
    QPointer<QWidget> thumbArea_;
    QPointer<QWidget> centerArea_;
    QPointer<QStackedLayout> centerLayout_;
    QPointer<PageSwitcher> pageSwitcher_;
    QPointer<QWidget> pageControllLayer_;
};

}
