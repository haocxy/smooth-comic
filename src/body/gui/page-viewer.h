#pragma once

#include <QSplitter>
#include <QPointer>
#include <QStackedLayout>


namespace myapp {
Q_NAMESPACE

class Book;


// 页面展示器
// 是页面展示区域的最外层组件
// 负责整合管理和页面显示、操作等有关的各GUI组件
class PageViewerWidget : public QSplitter {
    Q_OBJECT
public:
    explicit PageViewerWidget(Book &book, QWidget *parent = nullptr);

private:
    Book &book_;
    QPointer<QWidget> thumbArea_;
    QPointer<QWidget> centerArea_;
    QPointer<QStackedLayout> centerLayout_;
    QPointer<QWidget> pageSwitcher_;
    QPointer<QWidget> pageControllLayer_;
};

}
