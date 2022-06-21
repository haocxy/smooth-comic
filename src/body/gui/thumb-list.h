#pragma once

#include <QWidget>
#include <QScrollArea>
#include <QPointer>
#include <QBoxLayout>
#include <QPixmap>

#include "core/basetype.h"

#include "util/sigconns.h"
#include "util/qtobj-strand-entry.h"

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
    void addPageThumbnailItemWidget(const QString &entryPath, i32 width, i32 height);

    void removeAllThumbs();

protected:
    virtual void resizeEvent(QResizeEvent *) override;

private:
    Book &book_;

private:
    QPointer<JumpBar> jumpBar_;
    QPointer<QWidget> root_;
    QPointer<QVBoxLayout> layout_;
    QObjectList thumbWidgets_;
    QtObjStrandEntry strandEntry_;
    SigConns sigConns_;
    StrongHandle<ThumbList> handle_;
};


}
