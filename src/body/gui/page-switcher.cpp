#include "page-switcher.h"


namespace myapp {

PageSwitcher::PageSwitcher(Book &book, QWidget *parent)
    : QWidget(parent)
    , book_(book)
{
}

PageSwitcher::~PageSwitcher()
{

}

}
