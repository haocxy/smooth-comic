#pragma once

#include <QWidget>
#include <QLabel>
#include <QPointer>
#include <QPixmap>


namespace myapp {

class PageThumbnailItemWidget : public QWidget {
    Q_OBJECT
public:
    explicit PageThumbnailItemWidget(const QPixmap &img, const QString &text, QWidget *parent = nullptr);

    virtual ~PageThumbnailItemWidget() {}

    virtual void paintEvent(QPaintEvent *) override;

private:
    QPointer<QLabel> img_;
    QPointer<QLabel> text_;
};

}
