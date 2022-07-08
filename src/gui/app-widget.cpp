#include "app-widget.h"

#include <QFileDialog>

#include "core/system.h"

#include "book/book.h"

#include "controller/controller.h"

#include "title-bar-area/title-bar-area.h"
#include "book-area/book-area.h"
#include "controll-bar-area/controll-bar-area.h"


namespace myapp {

AppWidget::AppWidget(Engine &engine, QWidget *parent)
    : QWidget(parent)
    , engine_(engine)
{
    controller_ = new Controller(engine, this);

    resize(1000, 800);
    setMinimumSize(800, 600);

    initAreas();
}

AppWidget::~AppWidget()
{
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

    initControllBarArea();
}

void AppWidget::initTitleBarArea()
{
    titleBarArea_ = new TitleBarArea(this);
    rootLayout_->addWidget(titleBarArea_);
}

void AppWidget::initBookArea()
{
    bookArea_ = new BookArea(*controller_, this);
    rootLayout_->addWidget(bookArea_);
}

void AppWidget::initControllBarArea()
{
    controllBarArea_ = new ControllBarArea(*controller_, this);
    rootLayout_->addWidget(controllBarArea_);
}

void AppWidget::fileOpenAction()
{
    const fs::path home = SystemUtil::userHome();
    const QString defaultDir = QString::fromStdU32String(home.generic_u32string());
    const QString filePath = QFileDialog::getOpenFileName(this, tr("Open Comic"), defaultDir);

    controller_->book().open(filePath.toStdU32String());
}

void AppWidget::bookCloseAction()
{
    controller_->book().close();
}

void AppWidget::bookReloadAction()
{
    controller_->book().reload();
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
