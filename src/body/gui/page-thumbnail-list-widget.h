#pragma once

#include <QWidget>
#include <QScrollArea>
#include <QPointer>
#include <QBoxLayout>
#include <QPixmap>

#include "book/page-num.h"


namespace myapp {

class PageThumbnailListWidget : public QScrollArea {
    Q_OBJECT
public:
    explicit PageThumbnailListWidget(QWidget *parent = nullptr);

    virtual ~PageThumbnailListWidget() {}

    void addPageThumbnailItemWidget(PageNum pageNum, const QPixmap &img);

protected:
    virtual void resizeEvent(QResizeEvent *) override;

private:
    QPointer<QWidget> root_;
    QPointer<QVBoxLayout> layout_;
};


}
