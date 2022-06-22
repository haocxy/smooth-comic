#pragma once

#include <QWidget>
#include <QBoxLayout>
#include <QPointer>

#include "core/basetype.h"
#include "util/sigconns.h"
#include "util/qtobj-strand-entry.h"
#include "book/page-num.h"



namespace myapp {

class Book;

class ThumbList : public QWidget {
    Q_OBJECT
public:
    explicit ThumbList(Book &book, QWidget *parent = nullptr);

    virtual ~ThumbList();

protected:
    virtual void moveEvent(QMoveEvent *e) override;

private:
    void addThumbItem(PageNum seqNum, const QString &entryName, i32 imgRawWidth, i32 imgRawHeight);

    void removeAllThumbs();

private:
    Book &book_;

private:
    QPointer<QVBoxLayout> layout_;
    QObjectList thumbWidgets_;
    SigConns sigConns_;
    QtObjStrandEntry strandEntry_;
    StrongHandle<ThumbList> handle_;
};

}
