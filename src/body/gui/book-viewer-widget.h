#pragma once

#include <QWidget>
#include <QSplitter>
#include <QPointer>
#include <QStackedLayout>

#include "book/book.h"


namespace myapp {

class Engine;

class BookViewerWidget : public QSplitter {
    Q_OBJECT
public:
    explicit BookViewerWidget(Engine &engine, QWidget *parent = nullptr);

private:
    Engine &mEngine;

private: // UI Components
    QPointer<QWidget> artboard_;

private: // non-UI Components
    QPointer<Book> book_;
};

}
