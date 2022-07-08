#pragma once

#include <QFrame>
#include <QBoxLayout>
#include <QPointer>

#include "core/basetype.h"
#include "util/sigconns.h"
#include "gui-util/qtobj-strand-entry.h"
#include "book/page-num.h"



namespace myapp {

class Controller;

class ThumbItem;

class ThumbList : public QFrame {
    Q_OBJECT
public:
    explicit ThumbList(Controller &controller, QWidget *parent = nullptr);

    virtual ~ThumbList();

signals:
    void sigJumpTo(PageNum seqNum);

protected:
    virtual void moveEvent(QMoveEvent *e) override;

    virtual void resizeEvent(QResizeEvent *e) override;

private:
    void addThumbItem(PageNum seqNum, const QString &entryName, i32 imgRawWidth, i32 imgRawHeight);

    void removeAllThumbs();

    // 更新各个缩略项是否可见的状态
    // 当缩略项不可见时（或利可见区域足够远，取决于策略）时释放其中的缩略图，以节省内存
    void updateThumbsVisiableState();

private:
    Controller &controller_;

private:
    QPointer<QLayout> layout_;
    QList<QPointer<ThumbItem>> thumbWidgets_;
    SigConns sigConns_;
    QtObjStrandEntry strandEntry_;
    StrongHandle<ThumbList> handle_;
};

}
