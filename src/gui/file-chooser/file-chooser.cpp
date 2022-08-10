#include "file-chooser.h"

#include <QDebug>
#include <QDir>

#include "core/system.h"


namespace myapp {

FileChooser::FileChooser(QObject *parent)
    : QObject(parent)
{
    qDebug() << "FileChooser::FileChooser()";

    setCurrDir(QString::fromStdU32String(
        SystemUtil::defaultOpenFileDir().generic_u32string()
    ));
}

FileChooser::~FileChooser()
{
    qDebug() << "FileChooser::~FileChooser()";
}

void FileChooser::setCurrDir(const QString &dir)
{
    const fs::path newDir{ fs::absolute(dir.toStdU32String()) };
    if (!fs::is_directory(newDir)) {
        return;
    }
    if (currDir_ != newDir) {
        currDir_ = newDir;
        emit currDirChanged();
        updateEntries();
    }
}

void FileChooser::updateEntries()
{
    dirs_.clear();
    files_.clear();

    if (!fs::is_directory(currDir_)) {
        return;
    }

    for (QString name : QDir(currDir_).entryList(QDir::NoFilter, QDir::SortFlag::Name)) {
        const fs::path path = fs::absolute(currDir_ / name.toStdU32String());
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
