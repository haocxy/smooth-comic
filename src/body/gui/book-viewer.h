#pragma once

#include <QMainWindow>
#include <QSplitter>
#include <QPointer>
#include <QStackedLayout>

#include "book/book.h"


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
    QPointer<QWidget> thumbList_;
    QPointer<QWidget> pageViewer_;
    QPointer<QWidget> bookStatus_;
};

}
