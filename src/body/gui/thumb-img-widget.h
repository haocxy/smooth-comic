#pragma once

#include <QWidget>
#include <QPixmap>


namespace myapp {

class ThumbImgWidget : public QWidget {
    Q_OBJECT
public:
    explicit ThumbImgWidget(const QPixmap &img, QWidget *parent = nullptr);

    virtual ~ThumbImgWidget() {}

protected:
    virtual void paintEvent(QPaintEvent *) override;

private:
    QPixmap img_;
};

}
