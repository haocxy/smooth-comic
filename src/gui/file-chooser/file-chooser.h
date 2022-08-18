#pragma once

#include <vector>
#include <deque>

#include <QObject>
#include <QString>
#include <QList>

#include <QQmlListProperty>

#include "core/basetype.h"
#include "core/fs.h"
#include "file-chooser-entry.h"
#include "file-chooser-stack-info.h"


namespace myapp {

class FileChooser : public QObject {
    Q_OBJECT

        Q_PROPERTY(QString currDir READ currDir WRITE setCurrDir NOTIFY currDirChanged)

        Q_PROPERTY(QQmlListProperty<FileChooserEntry> dirs READ dirs NOTIFY dirsChanged)

        Q_PROPERTY(QQmlListProperty<FileChooserEntry> files READ files NOTIFY filesChanged)

        Q_PROPERTY(int historyStackLimit READ historyStackLimit WRITE setHistoryStackLimit NOTIFY historyStackLimitChanged)

public:
    explicit FileChooser(QObject *parent = nullptr);

    virtual ~FileChooser();

    QString currDir() const {
        return QString::fromStdU32String(currDir_.generic_u32string());
    }

    void setCurrDir(const QString &dir);

    void setCurrDir(const fs::path &dir);

    QQmlListProperty<FileChooserEntry> dirs() {
        return QQmlListProperty<FileChooserEntry>(
            this, nullptr, &FileChooser::dirCount, &FileChooser::dirAt);
    }

    QQmlListProperty<FileChooserEntry> files() {
        return QQmlListProperty<FileChooserEntry>(
            this, nullptr, &FileChooser::fileCount, &FileChooser::fileAt);
    }

    int historyStackLimit() const {
        return historyStackLimit_;
    }

    void setHistoryStackLimit(int limit);

    Q_INVOKABLE void openDir(const QString &path, qreal currContentY);

    Q_INVOKABLE void goBack();

signals:
    void currDirChanged();

    void dirsChanged();

    void filesChanged();

    void historyStackLimitChanged();

    void sigRestoreContentY(qreal contentY);

private:
    void updateEntries();

    static qsizetype dirCount(QQmlListProperty<FileChooserEntry> *list) {
        return reinterpret_cast<FileChooser *>(list->object)->dirs_.size();
    }

    static FileChooserEntry *dirAt(QQmlListProperty<FileChooserEntry> *list, qsizetype i) {
        return reinterpret_cast<FileChooser *>(list->object)->dirs_[i].get();
    }

    static qsizetype fileCount(QQmlListProperty<FileChooserEntry> *list) {
        return reinterpret_cast<FileChooser *>(list->object)->files_.size();
    }

    static FileChooserEntry *fileAt(QQmlListProperty<FileChooserEntry> *list, qsizetype i) {
        return reinterpret_cast<FileChooser *>(list->object)->files_[i].get();
    }

    void removeTooOldHistories() {
        while (historyStack_.size() > historyStackLimit_) {
            historyStack_.pop_front();
        }
    }

private:
    fs::path currDir_;

    std::vector<uptr<FileChooserEntry>> dirs_;
    std::vector<uptr<FileChooserEntry>> files_;

    int historyStackLimit_{ 20 };
    std::deque<uptr<FileChooserStackInfo>> historyStack_;
};

}
