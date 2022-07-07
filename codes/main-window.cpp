#include "main-window.h"

#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QToolBar>
#include <QDockWidget>
#include <QStatusBar>
#include <QFileDialog>
#include <QMouseEvent>
#include <QApplication>
#include <QHoverEvent>

#include <windows.h>
#include <WinUser.h>
#include <windowsx.h>
#include <dwmapi.h>
#include <objidl.h>
#include <gdiplus.h>
#include <GdiPlusColor.h>
#pragma comment (lib,"Dwmapi.lib")
#pragma comment (lib,"user32.lib")

#include "core/system.h"
#include "core/logger.h"

#include "book-area/book-area.h"

#include "util/my-button.h"


namespace myapp {

using logger::gLogger;

MainWindow::MainWindow(Engine &engine, QWidget *parent)
    : QMainWindow(parent)
    , engine_(engine)
{
    setWindowFlag(Qt::Widget);
    setWindowFlag(Qt::FramelessWindowHint);
    setWindowFlag(Qt::WindowSystemMenuHint);

    setWindowFlag(Qt::WindowMaximizeButtonHint, false);

    book_ = std::make_unique<Book>(engine_);

    resize(1000, 800);
    setMinimumSize(800, 600);

    initAreas();

    HWND hwnd = reinterpret_cast<HWND>(winId());
    DWORD style = ::GetWindowLong(hwnd, GWL_STYLE);
    ::SetWindowLong(hwnd, GWL_STYLE, style | WS_MAXIMIZEBOX | WS_THICKFRAME | WS_CAPTION);

    const MARGINS shadow = { 1, 1, 0, 1 };
    DwmExtendFrameIntoClientArea(hwnd, &shadow);
}

bool MainWindow::nativeEvent(const QByteArray &eventType, void *message, qintptr *result)
{
    MSG *msg = reinterpret_cast<MSG *>(message);

    switch (msg->message) {
    case WM_NCCALCSIZE:
    {
        NCCALCSIZE_PARAMS &params = *reinterpret_cast<NCCALCSIZE_PARAMS *>(msg->lParam);
        if (params.rgrc[0].top != 0) {
            params.rgrc[0].top -= 1;
        }

        *result = WVR_REDRAW;
        return true;
    }
    case WM_MOUSEMOVE:
    {
        *result = 0;
        

        long x = GET_X_LPARAM(msg->lParam);
        long y = GET_Y_LPARAM(msg->lParam);
        double dpr = this->devicePixelRatioF();
        QPoint gpos = QPoint(x / dpr, y / dpr);
        if (maxButton_->rect().contains(maxButton_->mapFromGlobal(gpos))) {
            gLogger.d << "WM_MOUSEMOVE";
        }
        *result = 0;
        return false;
    }
    case WM_NCLBUTTONDOWN:
    {
        *result = 0;

        long x = GET_X_LPARAM(msg->lParam);
        long y = GET_Y_LPARAM(msg->lParam);
        double dpr = this->devicePixelRatioF();
        QPoint gpos = QPoint(x / dpr, y / dpr);
        QPoint pos = mapFromGlobal(QPoint(x / dpr, y / dpr));
        if (maxButton_->rect().contains(maxButton_->mapFromGlobal(gpos))) {
            emit maxButton_->clicked();
            return true;
        }
        *result = 0;
        return false;
    }
    case WM_NCLBUTTONUP:
    {
        *result = 0;

        long x = GET_X_LPARAM(msg->lParam);
        long y = GET_Y_LPARAM(msg->lParam);
        double dpr = this->devicePixelRatioF();
        QPoint gpos = QPoint(x / dpr, y / dpr);
        QPoint pos = mapFromGlobal(QPoint(x / dpr, y / dpr));
        if (maxButton_->rect().contains(maxButton_->mapFromGlobal(gpos))) {
            *result = 0;
            return true;
        }
        *result = 0;
        return false;
    }
    case WM_NCHITTEST:
    {
        *result = 0;

        const LONG bw = borderWidth_;
        RECT r{};
        GetWindowRect(HWND(winId()), &r);

        long x = GET_X_LPARAM(msg->lParam);
        long y = GET_Y_LPARAM(msg->lParam);

        if (x >= r.left && x < r.left + bw &&
            y < r.bottom && y >= r.bottom - bw) {
            *result = HTBOTTOMLEFT;
            return true;
        }
        if (x < r.right && x >= r.right - bw &&
            y < r.bottom && y >= r.bottom - bw) {
            *result = HTBOTTOMRIGHT;
            return true;
        }
        if (x >= r.left && x < r.left + bw &&
            y >= r.top && y < r.top + bw) {
            *result = HTTOPLEFT;
            return true;
        }
        if (x < r.right && x >= r.right - bw &&
            y >= r.top && y < r.top + bw) {
            *result = HTTOPRIGHT;
            return true;
        }
        if (x >= r.left && x < r.left + bw) {
            *result = HTLEFT;
            return true;
        }
        if (x < r.right && x >= r.right - bw) {
            *result = HTRIGHT;
            return true;
        }

        if (y < r.bottom && y >= r.bottom - bw) {
            *result = HTBOTTOM;
            return true;
        }
        if (y >= r.top && y < r.top + bw) {
            *result = HTTOP;
            return true;
        }


        double dpr = this->devicePixelRatioF();
        QPoint gpos = QPoint(x / dpr, y / dpr);
        QPoint pos = mapFromGlobal(QPoint(x / dpr, y / dpr));

        if (maxButton_->rect().contains(maxButton_->mapFromGlobal(gpos))) {
            *result = HTMAXBUTTON;

            maxButton_->setStyleSheet("QPushButton{color:black;}");
            maxButtonHasStyle_ = true;

            gLogger.d << "WM_NCHITTEST";

            return true;
        } else {
            if (maxButtonHasStyle_) {
                maxButton_->setStyleSheet(QString());
                maxButtonHasStyle_ = false;
            }
        }

        if (menuBar()->rect().contains(pos)) {
            *result = HTCAPTION;
            return true;
        }

        return false;
    }
    case WM_GETMINMAXINFO:
    {
        if (::IsZoomed(msg->hwnd)) {
            RECT frame = { 0, 0, 0, 0 };
            AdjustWindowRectEx(&frame, WS_OVERLAPPEDWINDOW, FALSE, 0);

            double dpr = this->devicePixelRatioF();

            frames_.setLeft(abs(frame.left) / dpr + 0.5);
            frames_.setTop(abs(frame.bottom) / dpr + 0.5);
            frames_.setRight(abs(frame.right) / dpr + 0.5);
            frames_.setBottom(abs(frame.bottom) / dpr + 0.5);

            QMainWindow::setContentsMargins(
                frames_.left() + margins_.left(),
                frames_.top() + margins_.top(),
                frames_.right() + margins_.right(),
                frames_.bottom() + margins_.bottom()
            );
            justMaximized_ = true;
        } else {
            if (justMaximized_) {
                QMainWindow::setContentsMargins(margins_);
                frames_ = QMargins();
                justMaximized_ = false;
            }
        }
        return false;
    }
    default:
        return QMainWindow::nativeEvent(eventType, message, result);
    }
}

void MainWindow::initAreas()
{
    initMenuBarArea();
    initToolBarArea();
    initPageViewerArea();
    initStatusArea();
}

void MainWindow::initMenuBarArea()
{
    using Class = MainWindow;

    QMenuBar *bar = menuBar();

    QMenu *fileMenu = bar->addMenu(tr("File"));
    bind(fileMenu, tr("Open"), &Class::fileOpenAction, QKeySequence::StandardKey::Open);
    bind(fileMenu, tr("Close"), &Class::bookCloseAction, QKeySequence::StandardKey::Close);
    bind(fileMenu, tr("Reload"), &Class::bookReloadAction, QKeySequence::StandardKey::Refresh);

    QMenu *viewMenu = bar->addMenu(tr("View"));
    bind(viewMenu, tr("Next Page"), &Class::pageNextAction);
    bind(viewMenu, tr("Prev Page"), &Class::pagePrevAction);
}

void MainWindow::initToolBarArea()
{
    using Class = MainWindow;

    QToolBar *fileBar = addToolBar(tr("File"));
    bind(fileBar, tr("Open"), &Class::fileOpenAction);
    bind(fileBar, tr("Close"), &Class::bookCloseAction);
    bind(fileBar, tr("Reload"), &Class::bookReloadAction);

    QToolBar *viewBar = addToolBar(tr("View"));
    bind(viewBar, tr("Next Page"), &Class::pageNextAction);
    bind(viewBar, tr("Prev Page"), &Class::pagePrevAction);

    maxButton_ = new MyButton("Max Window");
    viewBar->addWidget(maxButton_);
    maxButton_->setFlat(true);
    connect(maxButton_, &QPushButton::clicked, this, [this] {
        if (isMaximized()) {
            showNormal();
            maxButton_->setText("Max Window");
        } else {
            showMaximized();
            maxButton_->setText("Normal Window");
        }
    });
}

void MainWindow::initPageViewerArea()
{
    bookArea_ = new BookArea(*book_, this);
    setCentralWidget(bookArea_);
}

void MainWindow::initStatusArea()
{
}

void MainWindow::bind(QMenu *menu, const QString &name, void(MainWindow:: *f)())
{
    QAction *action = menu->addAction(name);
    connect(action, &QAction::triggered, this, f);
}

void MainWindow::bind(QMenu *menu, const QString &name, void(MainWindow:: *f)(), const QKeySequence &shortcut)
{
    QAction *action = menu->addAction(name);
    action->setShortcut(shortcut);
    connect(action, &QAction::triggered, this, f);
}

void MainWindow::bind(QToolBar *toolBar, const QString &name, void(MainWindow:: *f)())
{
    QAction *action = toolBar->addAction(name);
    connect(action, &QAction::triggered, this, f);
}

void MainWindow::fileOpenAction()
{
    const fs::path home = SystemUtil::userHome();
    const QString defaultDir = QString::fromStdU32String(home.generic_u32string());
    const QString filePath = QFileDialog::getOpenFileName(this, tr("Open Comic"), defaultDir);

    book_->open(filePath.toStdU32String());
}

void MainWindow::bookCloseAction()
{
    book_->close();
}

void MainWindow::bookReloadAction()
{
    book_->reload();
}

void MainWindow::pageNextAction()
{
    bookArea_->jumpNext();
}

void MainWindow::pagePrevAction()
{
    bookArea_->jumpPrev();
}

}
