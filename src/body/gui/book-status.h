#pragma once

#include <QWidget>

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
};

}
