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

    connect(controller_, &Controller::cmdOpenComic, this, &AppWidget::fileOpenAction);
}

AppWidget::~AppWidget()
{
}

bool AppWidget::isWindowMoveAreaContainsGlobalPos(const QPoint &gpos)
{
    if (!titleBarArea_) {
        return false;
    }

    if (!titleBarArea_->rect().contains(titleBarArea_->mapFromGlobal(gpos))) {
        return false;
    }

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

    if (QFile(filePath).exists()) {
        controller_->book().open(filePath.toStdU32String());
    }
}

}
