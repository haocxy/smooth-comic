#include "page-switcher.h"

#include "core/debug-option.h"
#include "core/logger.h"

#include "book/book.h"


namespace myapp {

static DebugOption<bool> dopLog("log.need.page-switcher", true,
    "Is log for page switcher needed?");

using logger::gLogger;


PageSwitcher::PageSwitcher(Book &book, QWidget *parent)
    : QWidget(parent)
    , book_(book)
    , handle_(*this)
{
    sigConns_ += book_.sigBookClosed.connect([this, h = handle_.weak()](const fs::path &archiveFile) {
        h.apply([this] {
            strandEntry_.exec([this] {
                bookClosed();
            });
        });
    });

    sigConns_ += book_.sigPageLoaded.connect([this, h = handle_.weak()](const PageInfo &page) {
        h.apply([this, &page] {
            strandEntry_.exec([this, page] {
                pageLoaded(page);
            });
        });
    });
}

PageSwitcher::~PageSwitcher()
{

}

void PageSwitcher::bookClosed()
{
    loadedPages_.clear();
    waitingPage_.reset();
}

void PageSwitcher::pageLoaded(const PageInfo &page)
{
    loadedPages_[page.seqNum] = page;
}

}
