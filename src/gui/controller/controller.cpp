#include "controller.h"

#include <QFileDialog>

#include "core/system.h"

#include "book/book.h"


namespace myapp {

Controller::Controller(QObject *parent)
    : QObject(parent)
    , handle_(*this)
{

}

Controller::Controller(GuiEngine &engine, QObject *parent)
    : Controller(parent)
{
    setGuiEngine(&engine);
}

Controller::~Controller()
{
}

void Controller::setGuiEngine(GuiEngine *guiEngine) {
    if (guiEngine_ == nullptr) {
        guiEngine_ = guiEngine;

        book_ = new Book(guiEngine_->engine());
        sigConns_ += book_->sigBookOpenStarted.connect([this, h = handle_.weak()](const fs::path &p) {
            h.apply([this, &p] {
                strandEntry_.exec([this, p] {
                    filePath_ = QString::fromStdU32String(p.generic_u32string());
                    emit filePathChanged();
                });
            });
        });

        emit guiEngineChanged();
    }
}

void Controller::reloadComic()
{
    book_->reload();
}

void Controller::setFilePath(const QString &path)
{
    book_->open(fs::absolute(path.toStdU32String()));
}


}
