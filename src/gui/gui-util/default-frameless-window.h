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

    // 窗口的 窗口最大化按钮
    // 若窗口没有 窗口最大化按钮 则返回 nullptr
    virtual TitleBarButton *windowMaxButton() {
        return nullptr;
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
