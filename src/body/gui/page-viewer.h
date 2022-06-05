#pragma once

#include <QWidget>


namespace myapp {

class Book;

class PageViewerWidget : public QWidget {
    Q_OBJECT
public:
    explicit PageViewerWidget(Book &book, QWidget *parent = nullptr);

protected:
    virtual void paintEvent(QPaintEvent *) override;

private:
    Book &book_;
};

}
