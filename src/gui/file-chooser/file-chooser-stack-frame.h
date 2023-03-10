#pragma once

#include <vector>

#include <QObject>
#include <QQmlListProperty>

#include "core/basetype.h"
#include "core/fs.h"

#include "file-chooser-entry.h"


namespace myapp {

class FileChooserStackFrame : public QObject {
    Q_OBJECT

    Q_PROPERTY(QString path READ path NOTIFY pathChanged)

    Q_PROPERTY(QString name READ name NOTIFY nameChanged)

    Q_PROPERTY(QQmlListProperty<FileChooserEntry> dirs READ dirs NOTIFY dirsChanged)

    Q_PROPERTY(QQmlListProperty<FileChooserEntry> files READ files NOTIFY filesChanged)

public:
    explicit FileChooserStackFrame(const fs::path &dir, QObject *parent = nullptr);

    virtual ~FileChooserStackFrame();

    const fs::path &dir() const {
        return dir_;
    }

    const QString &path() const {
        return path_;
    }

    const QString &name() const {
        return name_;
    }

    QQmlListProperty<FileChooserEntry> dirs() {
        return QQmlListProperty<FileChooserEntry>(
            this, nullptr, &FileChooserStackFrame::dirCount, &FileChooserStackFrame::dirAt);
    }

    QQmlListProperty<FileChooserEntry> files() {
        return QQmlListProperty<FileChooserEntry>(
            this, nullptr, &FileChooserStackFrame::fileCount, &FileChooserStackFrame::fileAt);
    }

signals:
    void pathChanged();

    void nameChanged();

    void dirsChanged();

    void filesChanged();

private:
    static qsizetype dirCount(QQmlListProperty<FileChooserEntry> *list) {
        return reinterpret_cast<FileChooserStackFrame *>(list->object)->dirs_.size();
    }

    static FileChooserEntry *dirAt(QQmlListProperty<FileChooserEntry> *list, qsizetype i) {
        return reinterpret_cast<FileChooserStackFrame *>(list->object)->dirs_[i].get();
    }

    static qsizetype fileCount(QQmlListProperty<FileChooserEntry> *list) {
        return reinterpret_cast<FileChooserStackFrame *>(list->object)->files_.size();
    }

    static FileChooserEntry *fileAt(QQmlListProperty<FileChooserEntry> *list, qsizetype i) {
        return reinterpret_cast<FileChooserStackFrame *>(list->object)->files_[i].get();
    }

    void updateEntries();

private:
    const fs::path dir_;
    const QString path_;
    const QString name_;
    std::vector<uptr<FileChooserEntry>> dirs_;
    std::vector<uptr<FileChooserEntry>> files_;
};

}
