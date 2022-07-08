#pragma once

#include <QSplitter>
#include <QPointer>

#include "book/page-num.h"

#include "thumb-area/thumb-area.h"

#include "page-area/page-area.h"


namespace myapp {

class Book;

class Controller;


// 页面展示器
// 是页面展示区域的最外层组件
// 负责整合管理和页面显示、操作等有关的各GUI组件
class BookArea : public QSplitter {
    Q_OBJECT
public:
    explicit BookArea(Controller &controller, QWidget *parent = nullptr);

    void jumpTo(PageNum seqNum);

    void jumpNext();

    void jumpPrev();

private:
    QPointer<ThumbArea> thumbArea_;
    QPointer<QWidget> mainArea_;
    QPointer<QVBoxLayout> mainAreaLayout_;
    QPointer<PageArea> pageArea_;
    QPointer<QWidget> bookStatus_;
};

}
