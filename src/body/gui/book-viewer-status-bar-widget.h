#pragma once

#include <QWidget>

namespace Ui {

class BookViewerStatusBar;

}

namespace myapp {

class Book;

class BookViewerStatusBarWidget : public QWidget {
public:
    explicit BookViewerStatusBarWidget(Book &book, QWidget *parent = nullptr);

    virtual ~BookViewerStatusBarWidget();

private:
    Book &book_;
    Ui::BookViewerStatusBar *ui_{};
};

}
