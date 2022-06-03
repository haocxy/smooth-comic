#include "book-viewer-status-bar-widget.h"

#include "gen.book-viewer-status-bar.ui.h"


namespace myapp {



BookViewerStatusBarWidget::BookViewerStatusBarWidget(QWidget *parent)
    : ui_(new Ui::BookViewerStatusBar)
{
    ui_->setupUi(this);
}

BookViewerStatusBarWidget::~BookViewerStatusBarWidget()
{
    delete ui_;
    ui_ = nullptr;
}

}
