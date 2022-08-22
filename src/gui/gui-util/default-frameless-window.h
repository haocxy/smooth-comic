#pragma once

#include <QMainWindow>


namespace myapp {

class TitleBarButton;


class DefaultFramelessWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit DefaultFramelessWindow(QWidget *parent = nullptr)
        : QMainWindow(parent) {}

    virtual ~DefaultFramelessWindow() {}

    void setFramelessWindowMargins(const QMargins &margins) {}

protected:

    // 是否有 窗口的 窗口最大化按钮
    virtual bool hasWindowMaxButton() const {
        return false;
    }

    // 设置窗口的窗口最大化按钮的hover状态
    virtual void setWindowMaxButtonHover(bool hover) {
    }

    // 窗口最大化按钮被点击
    virtual void onWindowMaxButtonClicked() {
    }

    // 判断 窗口最大化按钮 是否包含指定的坐标
    // 注意！确保这个函数尽可能快，例如先根据标题栏的高度和布局快速判断几层，最后再具体查找
    virtual bool isWindowMaxButtonContainsGlobalPos(const QPoint &gpos) const {
        return false;
    }

    // 判断 窗口移动区域 是否包含指定的坐标
    // 注意！确保这个函数尽可能快，例如先根据标题栏的高度和布局快速判断几层，最后再具体查找
    virtual bool isWindowMoveAreaContainsGlobalPos(const QPoint &gpos) const {
        return false;
    }

};

}
