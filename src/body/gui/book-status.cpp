#include "book-status.h"

#include "book/book.h"
#include "gen.book-status.ui.h"


namespace myapp {



BookStatus::BookStatus(Book &book, QWidget *parent)
    : book_(book)
    , ui_(new Ui::BookStatus)
    , handle_(*this)
{
    ui_->setupUi(this);

    sigConns_ += book_.sigPageLoaded.connect([this, h = handle_.weak()](const PageInfo &page) {
        h.apply([this, &page] {
            strandEntry_.exec([this, page] {
                ++pageCount_;
                ui_->totalPageValue->setText(QString::number(pageCount_));
            });
        });
    });

    sigConns_ += book_.sigBookOpenStarted.connect([this, h = handle_.weak()](const fs::path &archiveFile) {
        h.apply([this, &archiveFile] {
            strandEntry_.exec([this, archiveFile] {
                pageCount_ = 0;
                ui_->totalPageValue->setText(QString::number(pageCount_));
            });
        });
    });
}

BookStatus::~BookStatus()
{
    delete ui_;
    ui_ = nullptr;
}

}
