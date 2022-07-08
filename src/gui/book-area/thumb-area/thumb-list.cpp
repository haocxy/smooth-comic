#include "thumb-list.h"

#include "core/logger.h"

#include "book/book.h"

#include "gui-util/auto-height-layout.h"

#include "controller/controller.h"

#include "thumb-item.h"


namespace myapp {

using logger::gLogger;

ThumbList::ThumbList(Controller &controller, QWidget *parent)
    : QWidget(parent)
    , controller_(controller)
    , handle_(*this)
{
    setObjectName("ThumbList");

    layout_ = new AutoHeightLayout(this);
    setLayout(layout_);

    sigConns_ += controller_.book().sigPageLoaded.connect([this, h = handle_.weak()](const PageInfo &page) {
        h.apply([this, &page] {
            strandEntry_.exec([this, page] {
                addThumbItem(page.seqNum, QString::fromStdU32String(page.name), page.rawWidth, page.rawHeight);
            });
        });
    });

    sigConns_ += controller_.book().sigBookOpenStarted.connect([this, h = handle_.weak()](const fs::path &archiveFile) {
        h.apply([this] {
            strandEntry_.exec([this] {
                removeAllThumbs();
            });
        });
    });

    sigConns_ += controller_.book().sigBookClosed.connect([this, h = handle_.weak()](const fs::path &archiveFile) {
        h.apply([this] {
            strandEntry_.exec([this] {
                removeAllThumbs();
            });
        });
    });
}

ThumbList::~ThumbList()
{
}

void ThumbList::moveEvent(QMoveEvent *e)
{
    QWidget::moveEvent(e);

    updateThumbsVisiableState();
}

void ThumbList::resizeEvent(QResizeEvent *e)
{
    QWidget::resizeEvent(e);

    updateThumbsVisiableState();
}

void ThumbList::addThumbItem(PageNum seqNum, const QString &entryName, i32 imgRawWidth, i32 imgRawHeight)
{
    ThumbItem *thumb = new ThumbItem(controller_.book(), seqNum, entryName.toStdU32String(), imgRawWidth, imgRawHeight, this);
    connect(thumb->img(), &ThumbImg::sigJumpTo, &controller_, &Controller::cmdJumpToPage);

    layout_->addWidget(thumb);

    thumbWidgets_.append(thumb);

    adjustSize();
}

void ThumbList::removeAllThumbs()
{
    for (QObject *thumb : thumbWidgets_) {
        delete thumb;
    }

    thumbWidgets_.clear();
}

void ThumbList::updateThumbsVisiableState()
{
    for (ThumbItem *thumb : thumbWidgets_) {
        thumb->updateThumbVisiableState();
    }
}

}
