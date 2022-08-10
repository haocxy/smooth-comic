#pragma once

#include <vector>

#include <QObject>
#include <QString>
#include <QList>

#include <QQmlListProperty>

#include "core/basetype.h"
#include "core/fs.h"
#include "file-chooser-entry.h"


namespace myapp {

class FileChooser : public QObject {
    Q_OBJECT

    Q_PROPERTY(QString currDir READ currDir WRITE setCurrDir NOTIFY currDirChanged)

    Q_PROPERTY(QQmlListProperty<FileChooserEntry> dirs READ dirs NOTIFY dirsChanged)

    Q_PROPERTY(QQmlListProperty<FileChooserEntry> files READ files NOTIFY filesChanged)
public:
    explicit FileChooser(QObject *parent = nullptr);

    virtual ~FileChooser();

    QString currDir() const {
        return QString::fromStdU32String(currDir_.generic_u32string());
    }

    void setCurrDir(const QString &dir);

    QQmlListProperty<FileChooserEntry> dirs() {
        return QQmlListProperty<FileChooserEntry>(
            this, nullptr, &FileChooser::dirCount, &FileChooser::dirAt);
    }

    QQmlListProperty<FileChooserEntry> files() {
        return QQmlListProperty<FileChooserEntry>(
            this, nullptr, &FileChooser::fileCount, &FileChooser::fileAt);
    }

signals:
    void currDirChanged();

    void dirsChanged();

    void filesChanged();

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

private:
    fs::path currDir_;

    std::vector<uptr<FileChooserEntry>> dirs_;
    std::vector<uptr<FileChooserEntry>> files_;
};

}
