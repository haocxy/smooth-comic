#pragma once

#include <QObject>

#include "core/declare_ptr.h"

#include "book/page-num.h"

#include "switch-direction.h"
#include "scale-mode.h"


namespace myapp {

class Engine;

class Book;

// 控制器
//
// 有很多功能都能够从多个地方触发，这样可以避免不同模块重复实现相同的功能，
// 也可以保证界面变化一致且易于维护
// 
// 所有 非展示性 的 GUI 操作（离如翻页、打开文件等）的控制中心
// 不要在 非展示性 的 GUI 模块间直接互相使用功能，而是通过控制器解除不同模块的耦合
class Controller : public QObject {
    Q_OBJECT
public:
    explicit Controller(Engine &engine, QObject *parent = nullptr);

    virtual ~Controller();

    Book &book() {
        return *book_;
    }

    void reloadComic();

signals:
    void cmdToggleThumbArea();

    void cmdOpenComic();

    void cmdRotatePageByOneStep();

    void cmdSwitchPage(SwitchDirection direction);

    void cmdJumpToPage(PageNum pageSeq);

    void cmdToggleScaleSettingPopup();

    void cmdSetScaleMode(ScaleMode mode);

    void cmdZoomIn();

    void cmdZoomOut();

    void sigScaleRangeUpdated(float min, float max);

    void sigScaleUpdated(float scale);

private:
    DeclarePtr<Book> book_;
};

}
