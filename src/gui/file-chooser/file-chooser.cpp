#include "file-chooser.h"

#include <QDebug>

#include "core/system.h"


namespace myapp {

FileChooser::FileChooser(QObject *parent)
    : QObject(parent)
{
    qDebug() << "FileChooser::FileChooser()";
}

FileChooser::~FileChooser()
{
    qDebug() << "FileChooser::~FileChooser()";
}

void FileChooser::openInitDir()
{

}

void FileChooser::openDir(const QString &path)
{
}

void FileChooser::goBack()
{
}

}
