#pragma once

#include <QMainWindow>
#include <QSplitter>
#include <QPointer>
#include <QStackedLayout>

#include "book/book.h"
#include "page-thumbnail-list-widget.h"


namespace myapp {

class Engine;

class BookViewerWidget : public QMainWindow {
    Q_OBJECT
public:
    explicit BookViewerWidget(Engine &engine, QWidget *parent = nullptr);

private:
    Engine &mEngine;

private: // UI Components
    QPointer<PageThumbnailListWidget> thumbnailList_;
    QPointer<QWidget> artboard_;

private: // non-UI Components
    QPointer<Book> book_;
};

}
