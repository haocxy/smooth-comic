#pragma once

#include <vector>

#include <QObject>
#include <QString>
#include <QList>

#include <QQmlListProperty>

#include "core/basetype.h"
#include "core/fs.h"
#include "file-tree-entry.h"


namespace myapp {

class FileTree : public QObject {
    Q_OBJECT

    Q_PROPERTY(QString currDir READ currDir WRITE setCurrDir NOTIFY currDirChanged)

    Q_PROPERTY(QQmlListProperty<FileTreeEntry> entries READ entries NOTIFY entriesChanged)
public:
    explicit FileTree(QObject *parent = nullptr);

    virtual ~FileTree();

    QString currDir() const {
        return QString::fromStdU32String(currDir_.generic_u32string());
    }

    void setCurrDir(const QString &dir);

    QQmlListProperty<FileTreeEntry> entries() {
        return QQmlListProperty<FileTreeEntry>(this, nullptr, &FileTree::entryCount, &FileTree::entry);
    }

signals:
    void currDirChanged();

    void entriesChanged();

private:
    void updateEntries();

    static qsizetype entryCount(QQmlListProperty<FileTreeEntry> *list) {
        return reinterpret_cast<FileTree *>(list->object)->entries_.size();
    }

    static FileTreeEntry *entry(QQmlListProperty<FileTreeEntry> *list, qsizetype i) {
        return reinterpret_cast<FileTree *>(list->object)->entries_[i].get();
    }

private:
    fs::path currDir_;

    std::vector<uptr<FileTreeEntry>> entries_;
};

}
