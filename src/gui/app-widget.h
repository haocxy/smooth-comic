#pragma once

#include <QWidget>
#include <QPointer>
#include <QBoxLayout>

#include "util/title-bar-button.h"

#include "book/book.h"


namespace myapp {

class Engine;

class TitleBarArea;

class BookArea;

class AppWidget : public QWidget {
    Q_OBJECT
public:
    explicit AppWidget(Engine &engine, QWidget *parent = nullptr);

    virtual ~AppWidget();

    TitleBarButton *windowFullScreenButton();

    TitleBarButton *windowMinButton();

    TitleBarButton *windowMaxButton();

    TitleBarButton *windowCloseButton();

    bool isWindowMoveAreaContainsGlobalPos(const QPoint &gpos);

private:
    void initAreas();

    void initTitleBarArea();

    void initBookArea();

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
    QPointer<QVBoxLayout> rootLayout_;
    QPointer<TitleBarArea> titleBarArea_;
    QPointer<BookArea> bookArea_;
};

}
