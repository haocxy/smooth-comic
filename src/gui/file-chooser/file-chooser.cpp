#include "file-chooser.h"

#include <QDebug>
#include <QDir>

#include "core/system.h"


namespace myapp {

FileChooser::FileChooser(QObject *parent)
    : QObject(parent)
{
    qDebug() << "FileChooser::FileChooser()";

    setCurrDir(QString::fromStdU32String(
        SystemUtil::defaultOpenFileDir().generic_u32string()
    ));
}

FileChooser::~FileChooser()
{
    qDebug() << "FileChooser::~FileChooser()";
}

void FileChooser::setCurrDir(const QString &dir)
{
    const fs::path newDir{ fs::absolute(dir.toStdU32String()) };
    setCurrDir(newDir);
}

void FileChooser::setCurrDir(const fs::path &dir)
{
    if (!fs::is_directory(dir)) {
        return;
    }
    if (currDir_ != dir) {
        currDir_ = dir;
        emit currDirChanged();
        updateEntries();
    }
}

void FileChooser::setHistoryStackLimit(int limit)
{
    if (historyStackLimit_ != limit) {
        historyStackLimit_ = limit;
        removeTooOldHistories();
        emit historyStackLimitChanged();
    }
}

void FileChooser::openDir(const QString &path, qreal currContentY)
{
    const fs::path p{ path.toStdU32String() };
    if (!fs::is_directory(p)) {
        return;
    }

    historyStack_.push_back(new FileChooserStackInfo(currDir_, currContentY));
    removeTooOldHistories();

    setCurrDir(path);
}

void FileChooser::goBack()
{
    if (historyStack_.empty()) {
        return;
    }

    uptr<FileChooserStackInfo> info = std::move(historyStack_.back());
    historyStack_.pop_back();

    // step 1: 清除当前的项
    dirs_.clear();
    emit dirsChanged();

    files_.clear();
    emit filesChanged();

    // step 2: 通知QML层开始恢复contentY属性
    // 注意这个信号的时序，必须晚于清除entry，且早于切换目录
    emit sigStartRestoreContentY(info->contentY());

    // step 3: 切换目录
    setCurrDir(info->dir());
}

void FileChooser::updateEntries()
{
    dirs_.clear();
    files_.clear();

    if (!fs::is_directory(currDir_)) {
        return;
    }

    for (QString name : QDir(currDir_).entryList(QDir::NoFilter, QDir::SortFlag::Name)) {
        const fs::path path = fs::absolute(currDir_ / name.toStdU32String());
        uptr<FileChooserEntry> entry = new FileChooserEntry(name, path);
        if (fs::is_directory(path)) {
            dirs_.push_back(std::move(entry));
        } else {
            files_.push_back(std::move(entry));
        }
    }

    emit dirsChanged();
    emit filesChanged();
}

}
