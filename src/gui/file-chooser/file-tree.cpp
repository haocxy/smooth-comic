#include "file-tree.h"

#include <QDebug>
#include <QDir>

#include "core/system.h"


namespace myapp {

FileTree::FileTree(QObject *parent)
    : QObject(parent)
{
    qDebug() << "FileTree::FileTree()";

    setCurrDir(QString::fromStdU32String(
        SystemUtil::defaultOpenFileDir().generic_u32string()
    ));
}

FileTree::~FileTree()
{
    qDebug() << "FileTree::~FileTree()";
}

void FileTree::setCurrDir(const QString &dir)
{
    const fs::path newDir{ fs::absolute(dir.toStdU32String()) };
    if (currDir_ != newDir) {
        currDir_ = newDir;
        emit currDirChanged();
        updateEntries();
    }
}

void FileTree::updateEntries()
{
    entries_.clear();

    if (!fs::is_directory(currDir_)) {
        return;
    }

    //for (const fs::directory_entry &e : fs::directory_iterator(currDir_)) {
    //    entries_.push_back(new FileTreeEntry(e.path().filename()));
    //}

    for (QString name : QDir(currDir_).entryList(QDir::NoFilter, QDir::SortFlag::Name)) {
        entries_.push_back(new FileTreeEntry(name));
    }

    emit entriesChanged();
}

}
