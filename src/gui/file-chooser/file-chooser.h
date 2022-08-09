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

    Q_PROPERTY(QQmlListProperty<FileChooserEntry> entries READ entries NOTIFY entriesChanged)
public:
    explicit FileChooser(QObject *parent = nullptr);

    virtual ~FileChooser();

    QString currDir() const {
        return QString::fromStdU32String(currDir_.generic_u32string());
    }

    void setCurrDir(const QString &dir);

    QQmlListProperty<FileChooserEntry> entries() {
        return QQmlListProperty<FileChooserEntry>(this, nullptr, &FileChooser::entryCount, &FileChooser::entry);
    }

signals:
    void currDirChanged();

    void entriesChanged();

private:
    void updateEntries();

    static qsizetype entryCount(QQmlListProperty<FileChooserEntry> *list) {
        return reinterpret_cast<FileChooser *>(list->object)->entries_.size();
    }

    static FileChooserEntry *entry(QQmlListProperty<FileChooserEntry> *list, qsizetype i) {
        return reinterpret_cast<FileChooser *>(list->object)->entries_[i].get();
    }

private:
    fs::path currDir_;

    std::vector<uptr<FileChooserEntry>> entries_;
};

}
