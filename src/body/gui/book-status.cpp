#include "book-status.h"

#include "book/book.h"
#include "gen.book-status.ui.h"


namespace myapp {



BookStatus::BookStatus(Book &book, QWidget *parent)
    : book_(book),
     ui_(new Ui::BookStatus)
{
    ui_->setupUi(this);

    connect(&book_, &Book::sigPageLoaded, this, [this](PageNum pageNum, const QPixmap &img, PageNum totalPages) {
        ui_->totalPageValue->setText(QString::number(totalPages));
    });
}

BookStatus::~BookStatus()
{
    delete ui_;
    ui_ = nullptr;
}

}
