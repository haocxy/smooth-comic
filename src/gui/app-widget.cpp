#include "app-widget.h"

#include <QFileDialog>

#include "core/system.h"

#include "title-bar-area/title-bar-area.h"
#include "book-area/book-area.h"


namespace myapp {

AppWidget::AppWidget(Engine &engine, QWidget *parent)
    : QWidget(parent)
    , engine_(engine)
{
    book_ = std::make_unique<Book>(engine_);

    resize(1000, 800);
    setMinimumSize(800, 600);

    initAreas();
}

AppWidget::~AppWidget()
{
}

TitleBarButton *AppWidget::windowFullScreenButton()
{
    return titleBarArea_->windowFullScreenButton();
}

TitleBarButton *AppWidget::windowMinButton()
{
    return titleBarArea_->windowMinButton();
}

TitleBarButton *AppWidget::windowMaxButton()
{
    return titleBarArea_->windowMaxButton();
}

TitleBarButton *AppWidget::windowCloseButton()
{
    return titleBarArea_->windowCloseButton();
}

bool AppWidget::isWindowMoveAreaContainsGlobalPos(const QPoint &gpos)
{
    return titleBarArea_->isWindowMoveAreaContainsGlobalPos(gpos);
}

void AppWidget::initAreas()
{
    setContentsMargins(QMargins());

    rootLayout_ = new QVBoxLayout(this);
    rootLayout_->setContentsMargins(QMargins());

    setLayout(rootLayout_);

    initTitleBarArea();

    initBookArea();
}

void AppWidget::initTitleBarArea()
{
    titleBarArea_ = new TitleBarArea(this);
    rootLayout_->addWidget(titleBarArea_);
}

void AppWidget::initBookArea()
{
    bookArea_ = new BookArea(*book_, this);
    rootLayout_->addWidget(bookArea_);
}

void AppWidget::fileOpenAction()
{
    const fs::path home = SystemUtil::userHome();
    const QString defaultDir = QString::fromStdU32String(home.generic_u32string());
    const QString filePath = QFileDialog::getOpenFileName(this, tr("Open Comic"), defaultDir);

    book_->open(filePath.toStdU32String());
}

void AppWidget::bookCloseAction()
{
    book_->close();
}

void AppWidget::bookReloadAction()
{
    book_->reload();
}

void AppWidget::pageNextAction()
{
    bookArea_->jumpNext();
}

void AppWidget::pagePrevAction()
{
    bookArea_->jumpPrev();
}

}
