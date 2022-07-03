#pragma once

#include <QWidget>
#include <QPointer>
#include <QStackedLayout>

#include "page-controll-layer.h"
#include "page-switcher.h"


namespace myapp {

class Book;

class PageArea : public QWidget {
    Q_OBJECT
public:
    explicit PageArea(Book &book, QWidget *parent = nullptr);

    virtual ~PageArea();

    void jumpTo(PageNum pageNum);

    void jumpNext();

    void jumpPrev();

private:
    QPointer<QStackedLayout> layout_;
    QPointer<PageControllLayer> controllLayer_;
    QPointer<PageSwitcher> switcher_;
};

}
