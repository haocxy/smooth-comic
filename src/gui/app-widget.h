#pragma once

#include <QWidget>
#include <QPointer>
#include <QBoxLayout>
#include <QStackedLayout>

#include "gui-util/title-bar-button.h"


namespace myapp {

class GuiEngine;

class PopupLayer;

class Controller;

class TitleBarArea;

class BookArea;

class ControllBarArea;



class AppWidget : public QWidget {
    Q_OBJECT
public:
    explicit AppWidget(GuiEngine &engine, QWidget *parent = nullptr);

    virtual ~AppWidget();

    TitleBarArea *titleBarArea() {
        return titleBarArea_;
    }

    bool isWindowMoveAreaContainsGlobalPos(const QPoint &gpos);

private:
    void initAreas();

    void initTitleBarArea();

    void initBookArea();

    void initControllBarArea();

private:
    void fileOpenAction();

private: // non-UI Components
    GuiEngine &engine_;
    QPointer<Controller> controller_;

private: // UI Components
    QPointer<QStackedLayout> layersLayout_;
    QPointer<PopupLayer> popupLayer_;
    QPointer<QWidget> mainLayer_;
    QPointer<QVBoxLayout> mainLayerLayout_;
    QPointer<TitleBarArea> titleBarArea_;
    QPointer<BookArea> bookArea_;
    QPointer<ControllBarArea> controllBarArea_;
};

}
