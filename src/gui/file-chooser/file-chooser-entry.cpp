#include "file-chooser-entry.h"


namespace myapp {

FileChooserEntry::FileChooserEntry(QObject *parent)
    : QObject(parent)
{
}

FileChooserEntry::FileChooserEntry(const QString &name, QObject *parent)
    : QObject(parent)
    , name_(name)
{
}

FileChooserEntry::~FileChooserEntry()
{

}

}
