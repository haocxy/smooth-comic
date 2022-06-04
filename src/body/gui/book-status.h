#pragma once

#include <QWidget>

namespace Ui {

class BookViewerStatusBar;

}

namespace myapp {

class Book;

class BookStatus : public QWidget {
public:
    explicit BookStatus(Book &book, QWidget *parent = nullptr);

    virtual ~BookStatus();

private:
    Book &book_;
    Ui::BookViewerStatusBar *ui_{};
};

}
