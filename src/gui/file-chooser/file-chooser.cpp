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
    entries_.clear();

    if (!fs::is_directory(currDir_)) {
        return;
    }

    for (QString name : QDir(currDir_).entryList(QDir::NoFilter, QDir::SortFlag::Name)) {
        const fs::path path = fs::absolute(currDir_ / name.toStdU32String());
        entries_.push_back(new FileChooserEntry(name, path));
    }

    emit entriesChanged();
}

}
