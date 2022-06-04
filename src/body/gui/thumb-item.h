#pragma once

#include <QWidget>
#include <QLabel>
#include <QPointer>
#include <QPixmap>

#include "thumb-img.h"


namespace myapp {

class ThumbItem : public QWidget {
    Q_OBJECT
public:
    explicit ThumbItem(PageNum pageNum, const QPixmap &img, const QString &text, QWidget *parent = nullptr);

    virtual ~ThumbItem() {}

private:
    PageNum pageNum_{};
    QPointer<ThumbImgWidget> img_;
    QPointer<QLabel> text_;
};

}
