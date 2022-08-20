#pragma once

#include <QQuickPaintedItem>
#include <QPointer>

#include "core/declare_ptr.h"

#include "gui/controller/controller.h"


namespace myapp {

class Controller;

class PageScene;


class PageReaderQuickItem : public QQuickPaintedItem {
    Q_OBJECT

    Q_PROPERTY(Controller *controller READ controller WRITE setController NOTIFY controllerChanged)

public:
    explicit PageReaderQuickItem(QQuickItem *parent = nullptr);

    virtual ~PageReaderQuickItem();

    virtual void paint(QPainter *painter) override;

    Controller *controller() {
        return controller_;
    }

    void setController(Controller *controller);

signals:
    void controllerChanged();

private:
    void updateSceneSize();

private:
    QPointer<Controller> controller_;
    DeclarePtr<PageScene> scene_;
};

}
