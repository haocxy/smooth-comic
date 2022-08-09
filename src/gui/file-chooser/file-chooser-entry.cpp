#include "file-chooser-entry.h"


namespace myapp {

FileTreeEntry::FileTreeEntry(QObject *parent)
    : QObject(parent)
{
}

FileTreeEntry::FileTreeEntry(const QString &name, QObject *parent)
    : QObject(parent)
    , name_(name)
{
}

FileTreeEntry::~FileTreeEntry()
{

}

}
