#pragma once

#include <QMainWindow>
#include <QSplitter>
#include <QPointer>
#include <QStackedLayout>
#include <QScrollArea>
#include <QPushButton>

#include "util/frameless-window.h"
#include "util/title-bar-button.h"

#include "book/book.h"


namespace myapp {

class Engine;

class BookArea;


class MainWindow : public FramelessWindow {
    Q_OBJECT
public:
    explicit MainWindow(Engine &engine, QWidget *parent = nullptr);

protected:
    virtual TitleBarButton *windowMaxButton() override;

    virtual bool isWindowMaxButtonContainsGlobalPos(const QPoint &gpos) const override;

    virtual bool isWindowMoveAreaContainsGlobalPos(const QPoint &gpos) const override;

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
    QPointer<TitleBarButton> windowMaxButton_;
};

}
