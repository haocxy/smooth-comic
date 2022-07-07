#include "frameless-window.win32.h"

#include <windows.h>
#include <WinUser.h>
#include <windowsx.h>
#include <dwmapi.h>
#include <objidl.h>
#include <gdiplus.h>
#include <GdiPlusColor.h>
#pragma comment (lib,"Dwmapi.lib")
#pragma comment (lib,"user32.lib")


namespace myapp {

FramelessWindow::FramelessWindow(QWidget *parent)
    : QMainWindow(parent)
{

    setWindowFlag(Qt::Widget);
    setWindowFlag(Qt::FramelessWindowHint);
    setWindowFlag(Qt::WindowSystemMenuHint);

    setWindowFlag(Qt::WindowMaximizeButtonHint, false);

    initWin32Window();
}

FramelessWindow::~FramelessWindow()
{
}

bool FramelessWindow::nativeEvent(const QByteArray &eventType, void *message, qintptr *result)
{
    MSG *msg = reinterpret_cast<MSG *>(message);

    Res optres;

    switch (msg->message) {
    case WM_GETMINMAXINFO:
        optres = handle_WM_GETMINMAXINFO(*msg);
        break;
    case WM_MOUSEMOVE:
        optres = handle_WM_MOUSEMOVE(*msg);
        break;
    case WM_NCCALCSIZE:
        optres = handle_WM_NCCALCSIZE(*msg);
        break;
    case WM_NCLBUTTONDOWN:
        optres = handle_WM_NCLBUTTONDOWN(*msg);
        break;
    case WM_NCLBUTTONUP:
        optres = handle_WM_NCLBUTTONUP(*msg);
        break;
    case WM_NCHITTEST:
        optres = handle_WM_NCHITTEST(*msg);
        break;
    default:
        return QMainWindow::nativeEvent(eventType, message, result);
    }

    if (optres) {
        *result = optres.val();
        return true;
    } else {
        return false;
    }
}

QPoint FramelessWindow::globalPos(const MSG &msg)
{
    const long x = GET_X_LPARAM(msg.lParam);
    const long y = GET_Y_LPARAM(msg.lParam);
    const qreal dpr = devicePixelRatioF();
    return QPoint(x / dpr, y / dpr);
}

void FramelessWindow::initWin32Window()
{
    HWND hwnd = reinterpret_cast<HWND>(winId());

    DWORD style = ::GetWindowLong(hwnd, GWL_STYLE);

    ::SetWindowLong(hwnd, GWL_STYLE,
        style | WS_MAXIMIZEBOX | WS_THICKFRAME | WS_CAPTION);


    MARGINS shadow{};
    shadow.cxLeftWidth = 1;
    shadow.cxRightWidth = 1;
    shadow.cyBottomHeight = 1;
    shadow.cyTopHeight = 0;
    // 顶部边距必须为0，否则无法正确处理 WM_NCHITTEST 消息

    DwmExtendFrameIntoClientArea(hwnd, &shadow);
}

namespace {

class Win32FrameSizeCvt {
public:
    Win32FrameSizeCvt(qreal dpr) : dpr_(dpr) {}

    int operator()(::LONG win32FrameSize) const {
        return std::abs(win32FrameSize) / dpr_ + 0.5;
    }

private:
    const qreal dpr_;
};

}

FramelessWindow::Res FramelessWindow::handle_WM_GETMINMAXINFO(MSG &msg)
{
    if (::IsZoomed(msg.hwnd)) {
        ::RECT frame{};
        AdjustWindowRectEx(&frame, WS_OVERLAPPEDWINDOW, FALSE, 0);

        const Win32FrameSizeCvt cvt(devicePixelRatioF());
        QMargins margins;
        margins.setTop(cvt(frame.top));
        margins.setRight(cvt(frame.right));
        margins.setBottom(cvt(frame.bottom));
        margins.setLeft(cvt(frame.left));

        normalMargins_ = contentsMargins();

        setContentsMargins(*normalMargins_ + margins);
    } else {
        if (normalMargins_) {
            setContentsMargins(*normalMargins_);
            normalMargins_ = std::nullopt;
        }
    }

    return Res();
}

FramelessWindow::Res FramelessWindow::handle_WM_MOUSEMOVE(MSG &msg)
{
    return Res();
}

FramelessWindow::Res FramelessWindow::handle_WM_NCCALCSIZE(MSG &msg)
{
    if (msg.wParam) {
        NCCALCSIZE_PARAMS &p = *reinterpret_cast<NCCALCSIZE_PARAMS *>(msg.lParam);
        if (p.rgrc[0].top > 0) {
            p.rgrc[0].top -= 1;
        }
        return WVR_REDRAW;
    }

    return Res();
}

FramelessWindow::Res FramelessWindow::handle_WM_NCLBUTTONDOWN(MSG &msg)
{
    return Res();
}

FramelessWindow::Res FramelessWindow::handle_WM_NCLBUTTONUP(MSG &msg)
{
    return Res();
}

FramelessWindow::Res FramelessWindow::handle_WM_NCHITTEST(MSG &msg)
{
    HWND hwnd = reinterpret_cast<HWND>(winId());

    ::RECT r{};
    ::GetWindowRect(hwnd, &r);

    using Win32Size = decltype(r.left);

    const Win32Size w = resizeAreaWidth_;
    const Win32Size x = GET_X_LPARAM(msg.lParam);
    const Win32Size y = GET_Y_LPARAM(msg.lParam);

    // 判断位置是否位于调整窗口大小的区域
    if (x < r.left + w) { // 水平左
        if (y >= r.bottom - w) { // 左下
            return HTBOTTOMLEFT;
        } else if (y < r.top + w) { //左上
            return HTTOPLEFT;
        } else {
            return HTLEFT; // 左中
        }
    } else if (x >= r.right - w) { // 水平右
        if (y >= r.bottom - w) { // 右下
            return HTBOTTOMRIGHT;
        } else if (y < r.top + w) { // 右上
            return HTTOPRIGHT;
        } else {
            return HTRIGHT; // 右中
        }
    } else { // 水平中
        if (y >= r.bottom - w) { // 中下
            return HTBOTTOM;
        } else if (y < r.top + w) { // 中上
            return HTTOP;
        }
    }

    const QPoint gpos = globalPos(msg);

    // 判断位置是否位于 窗口最大化按钮 区域
    if (TitleBarButton *maxBtn = windowMaxButton()) {
        if (isWindowMaxButtonContainsGlobalPos(gpos)) {
            maxBtn->setMouseOver(true);
            isWindowMaxButtonMouseOvered_ = true;
            return HTMAXBUTTON;
        } else {
            if (isWindowMaxButtonMouseOvered_) {
                maxBtn->setMouseOver(false);
                isWindowMaxButtonMouseOvered_ = false;
            }
        }
    }

    // 判断位置是否位于 窗口移动 区域
    if (isWindowMoveAreaContainsGlobalPos(gpos)) {
        return HTCAPTION;
    }

    return Res();
}

}
