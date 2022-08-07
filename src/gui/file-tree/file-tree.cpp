#include "file-tree.h"

#include <QDebug>

#include "core/system.h"


namespace myapp {

FileTree::FileTree(QObject *parent)
    : QObject(parent)
{
    qDebug() << "FileTree::FileTree()";

    currDir_ = QString::fromStdU32String(
        SystemUtil::defaultOpenFileDir().generic_u32string()
    );
}

FileTree::~FileTree()
{
    qDebug() << "FileTree::~FileTree()";
}

void FileTree::setCurrDir(const QString &dir)
{
    if (currDir_ != dir) {
        currDir_ = dir;

        emit currDirChanged();
    }
}

}
