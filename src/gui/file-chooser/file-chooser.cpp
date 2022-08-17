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
    const fs::path initDir = SystemUtil::defaultOpenFileDir();
    stack_.push(initDir);
}

void FileChooser::openDir(const QString &path)
{
    const fs::path p{ fs::absolute(path.toStdU32String()) };

    if (FileChooserStackFrame *topFrame = stack_.topFrame()) {
        if (topFrame->dir() == p) {
            return;
        }
    }

    stack_.push(p);
}

void FileChooser::goBack()
{
    if (stack_.frameCount() > 1) {
        stack_.pop();
    }
}

}
