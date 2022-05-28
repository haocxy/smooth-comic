#pragma once

#include <QWidget>
#include <QPointer>
#include <QStackedLayout>

#include "book/book.h"


namespace myapp {

class BookViewerWidget : public QWidget {
    Q_OBJECT
public:
    explicit BookViewerWidget(QWidget *parent = nullptr);

private: // UI Components
    QPointer<QStackedLayout> mLayout;
    QPointer<QWidget> mArtboard;

private: // non-UI Components
    QPointer<Book> mBook;
};

}
