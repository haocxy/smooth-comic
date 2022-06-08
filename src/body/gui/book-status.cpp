#include "book-status.h"

#include "book/book.h"
#include "gen.book-status.ui.h"


namespace myapp {



BookStatus::BookStatus(Book &book, QWidget *parent)
    : book_(book),
     ui_(new Ui::BookStatus)
{
    ui_->setupUi(this);

    connect(&book_, &Book::sigPageLoaded, this, [this](const QString &entryPath, i32 width, i32 height) {
        ++pageCount_;
        ui_->totalPageValue->setText(QString::number(pageCount_));
    });

    connect(&book_, &Book::sigBookOpenStarted, this, [this](const QString &archivePath) {
        pageCount_ = 0;
        ui_->totalPageValue->setText(QString::number(pageCount_));
    });
}

BookStatus::~BookStatus()
{
    delete ui_;
    ui_ = nullptr;
}

}
