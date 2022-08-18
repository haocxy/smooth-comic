#include "file-chooser-stack-info.h"


namespace myapp {

FileChooserStackInfo::FileChooserStackInfo(const fs::path &dir, qreal contentY, QObject *parent)
    : QObject(parent)
    , dir_(dir)
    , contentY_(contentY)
{
}

FileChooserStackInfo::~FileChooserStackInfo()
{

}

}
