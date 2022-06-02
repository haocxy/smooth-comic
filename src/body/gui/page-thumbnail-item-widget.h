#pragma once

#include <QWidget>
#include <QLabel>
#include <QPointer>
#include <QPixmap>

#include "thumb-img-widget.h"


namespace myapp {

class PageThumbnailItemWidget : public QWidget {
    Q_OBJECT
public:
    explicit PageThumbnailItemWidget(const QPixmap &img, const QString &text, QWidget *parent = nullptr);

    virtual ~PageThumbnailItemWidget() {}

private:
    QPointer<ThumbImgWidget> img_;
    QPointer<QLabel> text_;
};

}
