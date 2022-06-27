#include "page-switcher.h"

#include "core/debug-option.h"
#include "core/logger.h"

#include "book/book.h"

#include "page-widget.h"


namespace myapp {

static DebugOption<bool> dopLog("log.need.page-switcher", true,
    "Is log for page switcher needed?");

using logger::gLogger;


PageSwitcher::PageSwitcher(Book &book, QWidget *parent)
    : QWidget(parent)
    , book_(book)
    , handle_(*this)
{
    pageLayout_ = new QVBoxLayout(this);
    setLayout(pageLayout_);

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

    waitingPage_ = 0;
}

PageSwitcher::~PageSwitcher()
{

}

void PageSwitcher::jumpTo(PageNum pageNum)
{
    if (!loadedPages_.contains(pageNum)) {
        return;
    }

    asyncLoadImg(pageNum);
}

void PageSwitcher::jumpNext()
{
    if (pageWidget_) {
        jumpTo(pageWidget_->seqNum() + 1);
    }
}

void PageSwitcher::jumpPrev()
{
    if (pageWidget_) {
        jumpTo(pageWidget_->seqNum() - 1);
    }
}

void PageSwitcher::switchNextPage(SwitchDirection direction)
{
    switch (direction) {
    case SwitchDirection::Left:
        jumpNext();
        break;
    case SwitchDirection::Right:
        jumpPrev();
        break;
    case SwitchDirection::Up:
        jumpPrev();
        break;
    case SwitchDirection::Down:
        jumpNext();
        break;
    default:
        break;
    }
}

void PageSwitcher::bookClosed()
{
    loadedPages_.clear();
    waitingPage_.reset();
}

void PageSwitcher::pageLoaded(const PageInfo &page)
{
    loadedPages_[page.seqNum] = page;

    if (waitingPage_ && *waitingPage_ == page.seqNum) {
        asyncLoadImg(*waitingPage_);
    }
}

void PageSwitcher::asyncLoadImg(PageNum seqNum)
{
    book_.loadPageImg(seqNum, [this, seqNum, h = handle_.weak()](const QPixmap &img) {
        h.apply([this, &seqNum, &img] {
            strandEntry_.exec([this, seqNum, img] {
                onLoadPageImgDone(seqNum, img);
            });
        });
    });
}

void PageSwitcher::onLoadPageImgDone(PageNum seqNum, const QPixmap &img)
{
    if (waitingPage_ && *waitingPage_ != seqNum) {
        return;
    }

    waitingPage_ = std::nullopt;

    if (pageWidget_) {
        delete pageWidget_;
        pageWidget_ = nullptr;
    }

    pageWidget_ = new PageWidget(book_, seqNum, img, this);
    pageLayout_->addWidget(pageWidget_);
}

}
