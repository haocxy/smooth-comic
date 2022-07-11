#include "page-director.h"

#include <QPixmap>
#include <QPainter>

#include "book/book.h"

#include "controller/controller.h"


namespace myapp {

PageDirector::PageDirector(Controller &controller, QObject *parent)
    : QObject(parent)
    , controller_(controller)
    , handle_(*this)
{
    sigConns_ += controller_.book().sigBookClosed.connect([this, h = handle_.weak()](const fs::path &archiveFile) {
        h.apply([this] {
            strandEntry_.exec([this] {
                reset();
                });
            });
    });

    sigConns_ += controller_.book().sigPageLoaded.connect([this, h = handle_.weak()](const PageInfo &page) {
        h.apply([this, &page] {
            strandEntry_.exec([this, page] {
                pageLoaded(page);
                });
            });
    });

    waitingPage_ = 0;

    connect(&controller_, &Controller::cmdJumpToPage, this, &PageDirector::jumpTo);
    connect(&controller_, &Controller::cmdSwitchPage, this, &PageDirector::switchNextPage);
}

PageDirector::~PageDirector()
{
}

void PageDirector::updateShowSize(const QSize &size)
{
    showSize_ = size;
}

void PageDirector::reset()
{
    loadedPages_.clear();
    waitingPage_ = 0;
}

void PageDirector::draw(QPainter &p) const
{
    p.drawLine(10, 10, showSize_.width() - 20, showSize_.height() - 20);
}

void PageDirector::jumpTo(PageNum pageNum)
{
    if (!loadedPages_.contains(pageNum)) {
        return;
    }

    asyncLoadImg(pageNum);
}

void PageDirector::jumpNext()
{
    // TODO
}

void PageDirector::jumpPrev()
{
    // TODO
}

void PageDirector::switchNextPage(SwitchDirection direction)
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

void PageDirector::pageLoaded(const PageInfo &page)
{
    loadedPages_[page.seqNum] = page;

    if (waitingPage_ && *waitingPage_ == page.seqNum) {
        asyncLoadImg(*waitingPage_);
    }
}

void PageDirector::asyncLoadImg(PageNum seqNum)
{
    controller_.book().loadPageImg(seqNum, [this, seqNum, h = handle_.weak()](const QPixmap &img) {
        h.apply([this, &seqNum, &img] {
            strandEntry_.exec([this, seqNum, img] {
                onLoadPageImgDone(seqNum, img);
                });
            });
    });
}

void PageDirector::onLoadPageImgDone(PageNum seqNum, const QPixmap &img)
{
    if (waitingPage_ && *waitingPage_ != seqNum) {
        return;
    }

    waitingPage_ = std::nullopt;

    // TODO
}

}
