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
    void initAreas();

    void initMenuBarArea();

    void initToolBarArea();

    void initThumbArea();

    void initPageViewerArea();

    void initStatusArea();

    void bind(QMenu *menu, const QString &name, void(BookViewer:: *f)());

    void bind(QMenu *menu, const QString &name, void(BookViewer:: *f)(), const QKeySequence &shortcut);

    void bind(QToolBar *toolBar, const QString &name, void(BookViewer:: *f)());

private:
    void fileOpenAction();

private: // non-UI Components
    Engine &engine_;
    uptr<Book> book_;

private: // UI Components
    QPointer<QWidget> thumbList_;
    QPointer<QWidget> pageViewer_;
    QPointer<QWidget> bookStatus_;
};

}
