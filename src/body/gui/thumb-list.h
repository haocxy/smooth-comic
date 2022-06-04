#pragma once

#include <QWidget>
#include <QScrollArea>
#include <QPointer>
#include <QBoxLayout>
#include <QPixmap>

#include "book/page-num.h"

#include "jump-bar.h"


namespace myapp {

class Book;

class ThumbList : public QScrollArea {
    Q_OBJECT
public:
    explicit ThumbList(Book &book, QWidget *parent = nullptr);

    virtual ~ThumbList() {}

private:
    void addPageThumbnailItemWidget(PageNum pageNum, const QPixmap &img);

protected:
    virtual void resizeEvent(QResizeEvent *) override;

private:
    Book &book_;

private:
    QPointer<JumpBar> jumpBar_;
    QPointer<QWidget> root_;
    QPointer<QVBoxLayout> layout_;
};


}
