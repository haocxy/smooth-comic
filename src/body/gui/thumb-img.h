#pragma once

#include <QWidget>
#include <QPixmap>

#include "core/basetype.h"

#include "book/page-num.h"


namespace myapp {

class ThumbImg : public QWidget {
    Q_OBJECT
public:
    explicit ThumbImg(const u32str &entryPath, i32 width, i32 height, QWidget *parent = nullptr);

    virtual ~ThumbImg() {}

    virtual int heightForWidth(int width) const override {
        return int(width * (float(height_) / float(width_)));
    }

protected:
    virtual void paintEvent(QPaintEvent *) override;

private:
    const u32str entryPath_;
    const i32 width_{};
    const i32 height_{};
};

}
