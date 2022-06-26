#pragma once

#include <QWidget>


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
    Book &book_;
};

}
