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

    //for (const fs::directory_entry &e : fs::directory_iterator(currDir_)) {
    //    entries_.push_back(new FileChooserEntry(e.path().filename()));
    //}

    for (QString name : QDir(currDir_).entryList(QDir::NoFilter, QDir::SortFlag::Name)) {
        entries_.push_back(new FileChooserEntry(name));
    }

    emit entriesChanged();
}

}
