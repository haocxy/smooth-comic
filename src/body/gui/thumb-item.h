#pragma once

#include <QWidget>
#include <QLabel>
#include <QPointer>
#include <QPixmap>

#include "core/basetype.h"
#include "core/ustr.h"

#include "thumb-img.h"


namespace myapp {

class Book;

class ThumbItem : public QWidget {
    Q_OBJECT
public:
    explicit ThumbItem(Book &book, PageNum seqNum, const u32str &entryPath, i32 width, i32 height, QWidget *parent = nullptr);

    virtual ~ThumbItem() {}

private:
    const u32str entryPath_;
    QPointer<ThumbImg> img_;
    QPointer<QLabel> text_;
};

}
