#pragma once

#include <QWidget>
#include <QPointer>
#include <QStackedLayout>


namespace myapp {

class Book;


// 页面展示器
// 是页面展示区域的最外层组件
// 负责整合管理和页面显示、操作等有关的各GUI组件
class PageViewerWidget : public QWidget {
    Q_OBJECT
public:
    explicit PageViewerWidget(Book &book, QWidget *parent = nullptr);

private:
    Book &book_;
    QPointer<QStackedLayout> layout_;
    QPointer<QWidget> pageSwitcher_;
    QPointer<QWidget> pageControllLayer_;
};

}
