#include "file-chooser-stack.h"


namespace myapp {

FileChooserStack::FileChooserStack(QObject *parent)
    : QObject(parent)
{
}

FileChooserStack::~FileChooserStack()
{

}

void FileChooserStack::push(const fs::path &dir)
{
    if (!fs::is_directory(dir)) {
        return;
    }

    frames_.push_back(new FileChooserStackFrame(dir));

    removeTooOldHistories(ShouldEmit::No);

    emit framesChanged();
}

void FileChooserStack::pop()
{
    if (!frames_.empty()) {
        frames_.pop_back();
        emit framesChanged();
    }
}

}
