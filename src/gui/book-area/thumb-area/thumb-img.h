#pragma once

#include <QWidget>
#include <QPixmap>

#include "core/basetype.h"
#include "gui-util/qtobj-strand-entry.h"
#include "book/page-num.h"


namespace myapp {

class Book;

class ThumbImg : public QWidget {
    Q_OBJECT
public:
    explicit ThumbImg(Book &book, PageNum seqNum, i32 width, i32 height, QWidget *parent = nullptr);

    virtual ~ThumbImg() {}

    virtual int heightForWidth(int width) const override {
        return int(width * (float(height_) / float(width_)));
    }

    void updateThumbVisiableState(bool isVisiable);

signals:
    void sigJumpTo(PageNum seqNum);

protected:
    virtual void paintEvent(QPaintEvent *) override;

    virtual void mouseReleaseEvent(QMouseEvent *e) override;

private:
    void onThumbVisiableStateSwithed(opt<bool> oldIsVisiable, bool newIsVisiable);

    void loadImg();

    void releaseImg();

private:
    Book &book_;
    const PageNum seqNum_;
    const i32 width_{};
    const i32 height_{};
    QtObjStrandEntry strandEntry_;
    
    QPixmap img_;
    bool isImgLoading_{ false };

    opt<bool> isVisiable_;

    StrongHandle<ThumbImg> handle_;
};

}
