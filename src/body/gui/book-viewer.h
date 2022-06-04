#pragma once

#include <QMainWindow>
#include <QSplitter>
#include <QPointer>
#include <QStackedLayout>

#include "book/book.h"
#include "page-thumbnail-list-widget.h"


namespace myapp {

class Engine;

class BookViewer : public QMainWindow {
    Q_OBJECT
public:
    explicit BookViewer(Engine &engine, QWidget *parent = nullptr);

private:
    Engine &mEngine;

private: // non-UI Components
    QPointer<Book> book_;

private: // UI Components
    QPointer<PageThumbnailListWidget> thumbnailList_;
    QPointer<QWidget> pageViewer_;
    QPointer<QWidget> statusBar_;
};

}
