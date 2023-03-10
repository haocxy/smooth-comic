#include "book-status.h"

#include "book/book.h"

#include "gui-util/property-name.h"
#include "controller/controller.h"

#include "gen.book-status.ui.h"


namespace {

namespace prop {

using PropName = myapp::gui::PropertyName;

const PropName hasError{ "hasError" };

}

}


namespace myapp {


BookStatus::BookStatus(Controller &controller, QWidget *parent)
    : controller_(controller)
    , ui_(new Ui::BookStatus)
    , handle_(*this)
{
    ui_->setupUi(this);

    sigConns_ += controller_.book().sigPageLoaded.connect([this, h = handle_.weak()](const PageInfo &page) {
        h.apply([this, &page] {
            strandEntry_.exec([this, page] {
                ++pageCount_;
                ui_->totalPageValue->setText(QString::number(pageCount_));
            });
        });
    });

    sigConns_ += controller_.book().sigBookOpenStarted.connect([this, h = handle_.weak()](const fs::path &archiveFile) {
        h.apply([this, &archiveFile] {
            strandEntry_.exec([this, archiveFile] {
                reset(ShouldClearMsg::Yes);
            });
        });
    });

    sigConns_ += controller_.book().sigBookClosed.connect([this, h = handle_.weak()](const fs::path &archiveFile) {
        h.apply([this, &archiveFile] {
            strandEntry_.exec([this, archiveFile] {
                reset(ShouldClearMsg::No);
            });
        });
    });

    sigConns_ += controller_.book().sigLoadFailed.connect([this, h = handle_.weak()](BookError err) {
        h.apply([this, &err] {
            strandEntry_.exec([this, err] {
                onLoadError(err);
            });
        });
    });
}

BookStatus::~BookStatus()
{
    delete ui_;
    ui_ = nullptr;
}

void BookStatus::reset(ShouldClearMsg shouldClearMsg)
{
    pageCount_ = 0;
    ui_->totalPageValue->setText(QString::number(pageCount_));

    if (shouldClearMsg == ShouldClearMsg::Yes) {
        setProperty(prop::hasError, false);
        ui_->msgValue->clear();
    }
}

void BookStatus::onLoadError(BookError err)
{
    setProperty(prop::hasError, true);

    ui_->msgValue->setText(loadErrorToStr(err));
}

QString BookStatus::loadErrorToStr(BookError err)
{
    switch (err) {
    case BookError::BadArchive:
        return tr("Unsupported archive file");
    default:
        return QString();
    }
}

}
