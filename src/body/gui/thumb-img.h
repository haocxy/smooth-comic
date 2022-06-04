#pragma once

#include <QWidget>
#include <QPixmap>

#include "book/page-num.h"


namespace myapp {

class ThumbImgWidget : public QWidget {
    Q_OBJECT
public:
    explicit ThumbImgWidget(PageNum pageNum, const QPixmap &img, QWidget *parent = nullptr);

    virtual ~ThumbImgWidget() {}

    virtual int heightForWidth(int width) const override {
        return int(width * (float(img_.height()) / float(img_.width())));
    }

protected:
    virtual void paintEvent(QPaintEvent *) override;

private:
    PageNum pageNum_{};
    QPixmap img_;
};

}
