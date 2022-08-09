#include "file-chooser-entry.h"


namespace myapp {

FileChooserEntry::FileChooserEntry(QObject *parent)
    : QObject(parent)
{
}

FileChooserEntry::FileChooserEntry(const QString &name, const fs::path &path, QObject *parent)
    : QObject(parent)
{
    name_ = name;

    path_ = QString::fromStdU32String(path.generic_u32string());
}

FileChooserEntry::~FileChooserEntry()
{

}

}
