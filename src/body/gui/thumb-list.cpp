#include "thumb-list.h"

#include "core/logger.h"

#include "book/book.h"

#include "thumb-item.h"


namespace myapp {

using logger::gLogger;

ThumbList::ThumbList(Book &book, QWidget *parent)
    : QWidget(parent)
    , book_(book)
    , handle_(*this)
{
    layout_ = new QVBoxLayout(this);
    setLayout(layout_);

    layout_->addStretch();

    sigConns_ += book_.sigPageLoaded.connect([this, h = handle_.weak()](const PageInfo &page) {
        h.apply([this, &page] {
            strandEntry_.exec([this, page] {
                addThumbItem(page.seqNum, QString::fromStdU32String(page.name), page.rawWidth, page.rawHeight);
            });
        });
    });

    sigConns_ += book_.sigBookOpenStarted.connect([this, h = handle_.weak()](const fs::path &archiveFile) {
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
    ThumbItem *thumb = new ThumbItem(book_, seqNum, entryName.toStdU32String(), imgRawWidth, imgRawHeight, this);

    layout_->insertWidget(thumbWidgets_.size(), thumb);

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
