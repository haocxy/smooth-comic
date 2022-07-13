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

    // 如果有正在准备的场景，则不进行跳转，
    // 直到准备中的场景完成准备并切换为当前场景才可以跳转
    if (preparingScene_) {
        return;
    }

    preparingScene_ = new PageScene(controller_, pageNum);

    connect(preparingScene_, &PageScene::sigPrimaryPagePrepared, this, [this] {

        primaryScene_ = std::move(preparingScene_);
        primaryScene_->updateSceneSize(showSize_);
        primaryScene_->setIsPrimaryScene(true);

        connect(primaryScene_, &PageScene::cmdUpdate, this, &PageDirector::cmdUpdate);

        emit cmdUpdate();
    });
}

void PageDirector::jumpNext()
{
    if (primaryScene_) {
        jumpTo(primaryScene_->primaryPageSeq() + 1);
    }
}

void PageDirector::jumpPrev()
{
    if (primaryScene_) {
        jumpTo(primaryScene_->primaryPageSeq() - 1);
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

    // TODO 改成由 Book 对象用一个专用的信号通知初始页
    if (0 == page.seqNum) {
        jumpTo(0);
    }
}

}
