#include "gui-book.h"

#include <QDebug>

#include "book/book.h"


namespace myapp {

GuiBook::GuiBook(QObject *parent)
    : QObject(parent)
{
    qDebug() << "GuiBook::GuiBook()";
}

GuiBook::~GuiBook()
{
    qDebug() << "GuiBook::~GuiBook()";
}

void GuiBook::setGuiEngine(GuiEngine *guiEngine)
{
    if (guiEngine_.get() != guiEngine) {
        guiEngine_ = guiEngine;

        book_ = new Book(guiEngine_->engine());
        qDebug() << "GuiBook::setGuiEngine() book_ initiated";

        emit guiEngineChanged();
    }
}

const QString &GuiBook::file() const
{
    return file_;
}

void GuiBook::setFile(const QString &file)
{
    if (file_ != file) {
        file_ = file;
        book_->open(file_.toStdU32String());
        emit fileChanged();
    }
}

}
