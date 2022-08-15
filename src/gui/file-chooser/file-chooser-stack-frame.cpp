#include "file-chooser-stack-frame.h"

#include <QDir>


namespace myapp {

FileChooserStackFrame::FileChooserStackFrame(const fs::path &dir, QObject *parent)
    : QObject(parent)
    , dir_(dir)
    , path_(QString::fromStdU32String(dir_.generic_u32string()))
    , name_(QString::fromStdU32String(dir_.filename().generic_u32string()))
{
    updateEntries();
}

FileChooserStackFrame::~FileChooserStackFrame()
{
    int n = 0;
}

void FileChooserStackFrame::updateEntries()
{
    dirs_.clear();
    files_.clear();

    if (!fs::is_directory(dir_)) {
        return;
    }

    for (QString name : QDir(dir_).entryList(QDir::NoFilter, QDir::SortFlag::Name)) {
        const fs::path path = fs::absolute(dir_ / name.toStdU32String());
        uptr<FileChooserEntry> entry = new FileChooserEntry(name, path);
        if (fs::is_directory(path)) {
            dirs_.push_back(std::move(entry));
        } else {
            files_.push_back(std::move(entry));
        }
    }

    emit dirsChanged();
    emit filesChanged();
}

}
