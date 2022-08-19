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

    Q_PROPERTY(bool canGoBack READ canGoBack NOTIFY canGoBackChanged)

    Q_PROPERTY(QQmlListProperty<FileChooserEntry> entries READ entries NOTIFY entriesChanged)

    Q_PROPERTY(int historyStackLimit READ historyStackLimit WRITE setHistoryStackLimit NOTIFY historyStackLimitChanged)

public:
    explicit FileChooser(QObject *parent = nullptr);

    virtual ~FileChooser();

    QString currDir() const {
        return QString::fromStdU32String(currDir_.generic_u32string());
    }

    void setCurrDir(const QString &dir);

    void setCurrDir(const fs::path &dir);

    Q_INVOKABLE int entryCount() const {
        return entries_.size();
    }

    bool canGoBack() const {
        return !historyStack_.empty();
    }

    QQmlListProperty<FileChooserEntry> entries() {
        return QQmlListProperty<FileChooserEntry>(
            this, nullptr, &FileChooser::entryCount, &FileChooser::entryAt);
    }

    int historyStackLimit() const {
        return historyStackLimit_;
    }

    void setHistoryStackLimit(int limit);

    Q_INVOKABLE void openDir(const QString &path, qreal currContentY);

    Q_INVOKABLE void goBack();

signals:
    void currDirChanged();

    void canGoBackChanged();

    void entriesChanged();

    void historyStackLimitChanged();

    void sigStartRestoreContentY(qreal contentY);

private:
    void updateEntries();

    static qsizetype entryCount(QQmlListProperty<FileChooserEntry> *list) {
        return reinterpret_cast<FileChooser *>(list->object)->entries_.size();
    }

    static FileChooserEntry *entryAt(QQmlListProperty<FileChooserEntry> *list, qsizetype i) {
        return reinterpret_cast<FileChooser *>(list->object)->entries_[i].get();
    }

    void removeTooOldHistories() {
        const bool oldCanGoBack = !historyStack_.empty();
        while (historyStack_.size() > historyStackLimit_) {
            historyStack_.pop_front();
        }
        const bool newCanGoBack = !historyStack_.empty();
        if (oldCanGoBack != newCanGoBack) {
            emit canGoBackChanged();
        }
    }

private:
    fs::path currDir_;

    std::vector<uptr<FileChooserEntry>> entries_;

    int historyStackLimit_{ 100 };
    std::deque<uptr<FileChooserStackInfo>> historyStack_;
};

}
