#include "page-director.h"

#include <QPixmap>
#include <QPainter>

#include "book/book.h"

#include "controller/controller.h"

#include "page-scene.h"


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
    connect(&controller_, &Controller::cmdRotatePageByOneStep, this, &PageDirector::rotatePageByOneStep);
}

PageDirector::~PageDirector()
{
}

void PageDirector::updateShowSize(const QSize &size)
{
    showSize_ = size;

    if (primaryScene_) {
        primaryScene_->updateSceneSize(size);
    }
}

void PageDirector::reset()
{
    loadedPages_.clear();
    waitingPage_ = 0;
}

void PageDirector::draw(QPainter &p) const
{
    if (primaryScene_) {
        primaryScene_->draw(p);
    }
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
    if (currPage_) {
        jumpTo(*currPage_ + 1);
    }
}

void PageDirector::jumpPrev()
{
    if (currPage_) {
        jumpTo(*currPage_ - 1);
    }
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

void PageDirector::rotatePageByOneStep()
{
    if (primaryScene_) {
        primaryScene_->rotatePagesByOneStep();
        emit cmdUpdate();
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

    primaryScene_ = new PageScene;

    primaryScene_->updateSceneSize(showSize_);

    primaryScene_->addPage(seqNum, new PageSprite(img));

    currPage_ = seqNum;

    emit cmdUpdate();
}

}
