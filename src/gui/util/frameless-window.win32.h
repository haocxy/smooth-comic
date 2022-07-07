#pragma once

#include <QMainWindow>

#include "core/basetype.h"

#include "util/title-bar-button.h"


namespace myapp {

class FramelessWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit FramelessWindow(QWidget *parent = nullptr);

    virtual ~FramelessWindow();

    void setFramelessWindowMargins(const QMargins &margins);

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


protected:
    virtual bool nativeEvent(const QByteArray &eventType, void *message, qintptr *result) override;

    virtual void changeEvent(QEvent *e) override;

private:
    QPoint globalPos(const MSG &msg);

    void initWin32Window();

    void startMouseTrack(HWND hwnd);

    void stopMouseTrack(HWND hwnd);

    void updateFramelessWindowMargins();

    class Res {
    public:
        Res(): handled_(false) {}

        Res(qintptr val) : handled_(true), val_(val) {}

        operator bool() const {
            return handled_;
        }

        qintptr val() const {
            return val_;
        }

    private:
        bool handled_{ false };
        qintptr val_{};
    };

    Res handle_WM_GETMINMAXINFO(MSG &msg);

    Res handle_WM_MOUSEMOVE(MSG &msg);

    Res handle_WM_NCMOUSELEAVE(MSG &msg);

    Res handle_WM_NCCALCSIZE(MSG &msg);

    Res handle_WM_NCLBUTTONDOWN(MSG &msg);

    Res handle_WM_NCLBUTTONUP(MSG &msg);

    Res handle_WM_NCHITTEST(MSG &msg);

private:
    int resizeAreaWidth_{ 10 };
    int resizeAreaTopHeight_{ 5 };
    QMargins windowFrames_;
    QMargins margins_;
    bool isWindowRectAdjusted_{ false };
    bool isWindowMaxButtonMouseOvered_{ false };
    bool isTrackMouseStarted_{ false };
};

}
