#pragma once

#include <unordered_map>

#include <QWidget>
#include <QPointer>
#include <QBoxLayout>

#include "core/basetype.h"
#include "core/obj-handle.h"

#include "util/sigconns.h"
#include "gui-util/qtobj-strand-entry.h"

#include "book/page-num.h"
#include "book/page-info.h"

#include "controller/switch-direction.h"


namespace myapp {

class Controller;

class PageDirector;


// 页面切换器
// 是页面的管理器，管理页面的显示逻辑和页面的生命周期
// 一个页面切换器对象对应多个页面
class PageSwitcher : public QWidget {
    Q_OBJECT
public:
    explicit PageSwitcher(Controller &controller, QWidget *parent = nullptr);

    virtual ~PageSwitcher();

protected:
    virtual void resizeEvent(QResizeEvent *e) override;

    virtual void paintEvent(QPaintEvent *e) override;

private:

private:
    Controller &controller_;

    QtObjStrandEntry strandEntry_;

    QPointer<PageDirector> director_;

    StrongHandle<PageSwitcher> handle_;
};

}
