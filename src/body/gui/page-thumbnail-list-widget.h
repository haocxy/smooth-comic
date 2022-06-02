#pragma once

#include <QWidget>
#include <QScrollArea>
#include <QPointer>
#include <QBoxLayout>


namespace myapp {

class PageThumbnailListWidget : public QScrollArea {
    Q_OBJECT
public:
    explicit PageThumbnailListWidget(QWidget *parent = nullptr);

    virtual ~PageThumbnailListWidget() {}

private:
    QPointer<QVBoxLayout> layout_;
};


}
