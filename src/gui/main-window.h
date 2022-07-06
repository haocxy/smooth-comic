#pragma once

#include <QMainWindow>
#include <QSplitter>
#include <QPointer>
#include <QStackedLayout>
#include <QScrollArea>
#include <QPushButton>

#include "book/book.h"


namespace myapp {

class Engine;

class BookArea;


class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(Engine &engine, QWidget *parent = nullptr);

protected:

    virtual bool nativeEvent(const QByteArray &eventType, void *message, qintptr *result) override;

private:
    void initAreas();

    void initMenuBarArea();

    void initToolBarArea();

    void initPageViewerArea();

    void initStatusArea();

    void bind(QMenu *menu, const QString &name, void(MainWindow:: *f)());

    void bind(QMenu *menu, const QString &name, void(MainWindow:: *f)(), const QKeySequence &shortcut);

    void bind(QToolBar *toolBar, const QString &name, void(MainWindow:: *f)());



private:
    void fileOpenAction();

    void bookCloseAction();

    void bookReloadAction();

    void pageNextAction();

    void pagePrevAction();

private: // non-UI Components
    Engine &engine_;
    uptr<Book> book_;

private: // UI Components
    QPointer<BookArea> bookArea_;
    QPointer<QPushButton> maxButton_;

    int borderWidth_ = 5;
    QMargins margins_;
    QMargins frames_;
    bool justMaximized_ = false;
};

}
