#include "page-widget.h"


namespace myapp {

PageWidget::PageWidget(Book &book, PageNum seqNum, QWidget *parent)
    : QWidget(parent)
    , book_(book)
    , seqNum_(seqNum)
{
}

PageWidget::~PageWidget()
{
}

}
