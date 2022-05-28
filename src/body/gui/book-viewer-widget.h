#pragma once

#include <QWidget>
#include <QPointer>
#include <QStackedLayout>

#include "book/book.h"


namespace myapp {

class Engine;

class BookViewerWidget : public QWidget {
    Q_OBJECT
public:
    explicit BookViewerWidget(Engine &engine, QWidget *parent = nullptr);

private:
    Engine &mEngine;

private: // UI Components
    QPointer<QStackedLayout> mLayout;
    QPointer<QWidget> mArtboard;

private: // non-UI Components
    QPointer<Book> mBook;
};

}
