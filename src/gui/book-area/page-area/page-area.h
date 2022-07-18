#pragma once

#include <QWidget>
#include <QPointer>
#include <QStackedLayout>

#include "core/declare_ptr.h"

#include "page-controll-layer.h"
#include "page-switcher.h"


namespace myapp {

class Book;

class Controller;

class PageScene;


class PageArea : public QWidget {
    Q_OBJECT
public:
    explicit PageArea(Controller &controller, QWidget *parent = nullptr);

    virtual ~PageArea();

private:
    DeclarePtr<PageScene> scene_;
    QPointer<QStackedLayout> layout_;
    QPointer<PageControllLayer> controllLayer_;
    QPointer<PageSwitcher> switcher_;
};

}
