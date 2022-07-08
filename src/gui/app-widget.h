#pragma once

#include <QWidget>
#include <QPointer>
#include <QBoxLayout>

#include "gui-util/title-bar-button.h"


namespace myapp {

class Engine;

class TitleBarArea;

class BookArea;

class Controller;


class AppWidget : public QWidget {
    Q_OBJECT
public:
    explicit AppWidget(Engine &engine, QWidget *parent = nullptr);

    virtual ~AppWidget();

    TitleBarArea *titleBarArea() {
        return titleBarArea_;
    }

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
    QPointer<Controller> controller_;

private: // UI Components
    QPointer<QVBoxLayout> rootLayout_;
    QPointer<TitleBarArea> titleBarArea_;
    QPointer<BookArea> bookArea_;
};

}
