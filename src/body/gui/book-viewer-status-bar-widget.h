#pragma once

#include <QWidget>


namespace Ui {

class BookViewerStatusBar;

}

namespace myapp {

class BookViewerStatusBarWidget : public QWidget {
public:
    explicit BookViewerStatusBarWidget(QWidget *parent = nullptr);

    virtual ~BookViewerStatusBarWidget();

private:
    Ui::BookViewerStatusBar *ui_{};
};

}
