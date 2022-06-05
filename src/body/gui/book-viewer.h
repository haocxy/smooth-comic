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


private: // non-UI Components
    Engine &engine_;
    QPointer<Book> book_;

private: // UI Components
    QPointer<QWidget> thumbList_;
    QPointer<QWidget> pageViewer_;
    QPointer<QWidget> bookStatus_;
};

}
