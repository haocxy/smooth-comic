#include "book-viewer-status-bar-widget.h"

#include "book/book.h"
#include "gen.book-viewer-status-bar.ui.h"


namespace myapp {



BookViewerStatusBarWidget::BookViewerStatusBarWidget(Book &book, QWidget *parent)
    : book_(book),
     ui_(new Ui::BookViewerStatusBar)
{
    ui_->setupUi(this);

    connect(&book_, &Book::sigPageLoaded, this, [this](PageNum pageNum, const QPixmap &img, PageNum totalPages) {
        ui_->totalPageValue->setText(QString::number(totalPages));
    });
}

BookViewerStatusBarWidget::~BookViewerStatusBarWidget()
{
    delete ui_;
    ui_ = nullptr;
}

}