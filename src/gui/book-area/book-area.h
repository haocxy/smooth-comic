#pragma once

#include <QSplitter>
#include <QPointer>
#include <QStackedLayout>

#include "book/page-num.h"

#include "thumb/thumb-list-scroll-area.h"
#include "page-controll-layer.h"


namespace myapp {

class Book;

class PageSwitcher;


// 页面展示器
// 是页面展示区域的最外层组件
// 负责整合管理和页面显示、操作等有关的各GUI组件
class BookArea : public QSplitter {
    Q_OBJECT
public:
    explicit BookArea(Book &book, QWidget *parent = nullptr);

    void jumpTo(PageNum seqNum);

    void jumpNext();

    void jumpPrev();

private:
    Book &book_;
    QPointer<ThumbListScrollArea> thumbArea_;
    QPointer<QWidget> mainArea_;
    QPointer<QVBoxLayout> mainAreaLayout_;
    QPointer<QWidget> pageArea_;
    QPointer<QStackedLayout> pageAreaLayout_;
    QPointer<PageSwitcher> pageSwitcher_;
    QPointer<PageControllLayer> pageControllLayer_;
    QPointer<QWidget> bookStatus_;
};

}
