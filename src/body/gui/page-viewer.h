#pragma once

#include <QWidget>


namespace myapp {

class PageViewerWidget : public QWidget {
    Q_OBJECT
public:
    explicit PageViewerWidget(QWidget *parent = nullptr);

protected:
    virtual void paintEvent(QPaintEvent *) override;
};

}
