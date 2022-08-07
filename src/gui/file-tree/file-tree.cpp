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
    if (currDir_ != dir) {
        currDir_ = QString::fromStdU32String(fs::absolute(dir.toStdU32String()).generic_u32string());
        emit currDirChanged();
        updateEntries();
    }
}

void FileTree::updateEntries()
{
    entries_.clear();

    for (QString name : QDir(currDir_).entryList(QDir::NoFilter, QDir::SortFlag::Name)) {
        entries_.push_back(std::make_unique<FileTreeEntry>(name));
    }

    emit entriesChanged();
}

}
