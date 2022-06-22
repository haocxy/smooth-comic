#include "thumb-list-scroll-area.h"

#include "book/book.h"
#include "thumb-item.h"


namespace myapp {

static constexpr int widthHint = 200;

ThumbListScrollArea::ThumbListScrollArea(Book &book, QWidget *parent)
    : QScrollArea(parent)
    , book_(book)
    , handle_(*this)
{
    setHorizontalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOn);

    setMinimumWidth(widthHint);
    //setMaximumWidth(widthHint * 2);

    jumpBar_ = new JumpBar(this);
    setVerticalScrollBar(jumpBar_);

    root_ = new QWidget(this);

    layout_ = new QVBoxLayout(root_);
    root_->setLayout(layout_);

    layout_->addStretch();

    setWidget(root_);

    setWidgetResizable(true);

    sigConns_ += book_.sigPageLoaded.connect([this, h = handle_.weak()](const PageInfo &page) {
        h.apply([this, &page] {
            strandEntry_.exec([this, page] {
                addPageThumbnailItemWidget(page.seqNum, QString::fromStdU32String(page.name), page.rawWidth, page.rawHeight);
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

void ThumbListScrollArea::addPageThumbnailItemWidget(PageNum seqNum, const QString &entryPath, i32 width, i32 height)
{
    ThumbItem *itemWidget = new ThumbItem(book_, seqNum, entryPath.toStdU32String(), width, height, this);
    layout_->insertWidget(thumbWidgets_.size(), itemWidget);
    thumbWidgets_.append(itemWidget);
    root_->adjustSize();
}

void ThumbListScrollArea::removeAllThumbs()
{
    for (QObject *thumbWidget : thumbWidgets_) {
        delete thumbWidget;
    }

    thumbWidgets_.clear();
}

void ThumbListScrollArea::resizeEvent(QResizeEvent *)
{
    root_->adjustSize();
}


}
