#include "file-tree.h"

#include <QDebug>


namespace myapp {

FileTree::FileTree(QObject *parent)
    : QObject(parent)
{
    qDebug() << "FileTree::FileTree()";
}

FileTree::~FileTree()
{
    qDebug() << "FileTree::~FileTree()";
}

}
