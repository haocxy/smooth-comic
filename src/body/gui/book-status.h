#pragma once

#include <QWidget>

#include "core/basetype.h"

#include "util/sigconns.h"
#include "util/qtobj-strand-entry.h"


namespace Ui {

class BookStatus;

}

namespace myapp {

class Book;

class BookStatus : public QWidget {
public:
    explicit BookStatus(Book &book, QWidget *parent = nullptr);

    virtual ~BookStatus();

private:
    Book &book_;
    Ui::BookStatus *ui_{};
    i32 pageCount_{};
    QtObjStrandEntry strandEntry_;
    SigConns sigConns_;
    StrongHandle<BookStatus> handle_;
};

}
