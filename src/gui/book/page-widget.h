#pragma once

#include <QWidget>
#include <QPixmap>

#include "book/page-num.h"

namespace myapp {

class Book;

// 控制一个单独页面的显示
// 一个对象的全部生命周期所对应的页面不会变化
class PageWidget : public QWidget {
    Q_OBJECT
public:
    explicit PageWidget(Book &book, PageNum seqNum, const QPixmap &img, QWidget *parent = nullptr);

    virtual ~PageWidget();

    PageNum seqNum() const {
        return seqNum_;
    }

protected:
    virtual void paintEvent(QPaintEvent *e) override;

private:
    Book &book_;
    const PageNum seqNum_;
    const QPixmap img_;
};

}
