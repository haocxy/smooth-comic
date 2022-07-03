#pragma once

#include <QWidget>
#include <QScrollArea>
#include <QPointer>
#include <QBoxLayout>

#include "core/basetype.h"

#include "jump-bar.h"
#include "thumb-list.h"


namespace myapp {

class Book;


class ThumbListScrollArea : public QScrollArea {
    Q_OBJECT
public:
    explicit ThumbListScrollArea(Book &book, QWidget *parent = nullptr);

    virtual ~ThumbListScrollArea() {}

    ThumbList *thumbList() {
        return root_;
    }

protected:
    virtual void resizeEvent(QResizeEvent *) override;

private:
    Book &book_;

private:
    QPointer<JumpBar> jumpBar_;
    QPointer<ThumbList> root_;

    opt<double> scrollPercent_;
};


}
