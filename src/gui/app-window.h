#pragma once

#include <QMainWindow>
#include <QSplitter>
#include <QPointer>
#include <QBoxLayout>
#include <QStackedLayout>
#include <QScrollArea>
#include <QPushButton>

#include "util/frameless-window.h"
#include "util/title-bar-button.h"

#include "book/book.h"


namespace myapp {

class Engine;

class TitleBarArea;

class BookArea;


class AppWindow : public FramelessWindow {
    Q_OBJECT
public:
    explicit AppWindow(Engine &engine, QWidget *parent = nullptr);

protected:
    virtual TitleBarButton *windowMaxButton() override;

    virtual bool isWindowMaxButtonContainsGlobalPos(const QPoint &gpos) const override;

    virtual bool isWindowMoveAreaContainsGlobalPos(const QPoint &gpos) const override;

private:
    void initAreas();

    void initTitleBarArea();

    void initBookArea();

    void bind(QMenu *menu, const QString &name, void(AppWindow:: *f)());

    void bind(QMenu *menu, const QString &name, void(AppWindow:: *f)(), const QKeySequence &shortcut);

    void bind(QToolBar *toolBar, const QString &name, void(AppWindow:: *f)());

private:
    void toggleWindowMaxAction();

    void fileOpenAction();

    void bookCloseAction();

    void bookReloadAction();

    void pageNextAction();

    void pagePrevAction();

private: // non-UI Components
    Engine &engine_;
    uptr<Book> book_;

private: // UI Components
    QPointer<QWidget> rootWidget_;
    QPointer<QVBoxLayout> rootLayout_;
    QPointer<TitleBarArea> titleBarArea_;
    QPointer<BookArea> bookArea_;
};

}
