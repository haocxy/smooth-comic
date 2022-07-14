#pragma once

#include <QWidget>

#include "core/basetype.h"

#include "util/sigconns.h"
#include "gui-util/qtobj-strand-entry.h"

#include "book/book-error.h"


namespace Ui {

class BookStatus;

}

namespace myapp {

class Controller;

class BookStatus : public QWidget {
    Q_OBJECT
public:
    explicit BookStatus(Controller &controller, QWidget *parent = nullptr);

    virtual ~BookStatus();

private:
    enum class ShouldClearMsg {
        Yes, No
    };

    void reset(ShouldClearMsg shouldClearMsg);

    void onLoadError(BookError err);

    static QString loadErrorToStr(BookError err);

private:
    Controller &controller_;
    Ui::BookStatus *ui_{};
    i32 pageCount_{};
    QtObjStrandEntry strandEntry_;
    SigConns sigConns_;
    StrongHandle<BookStatus> handle_;
};

}
