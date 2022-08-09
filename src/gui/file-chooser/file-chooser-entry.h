#pragma once

#include <QObject>

#include "core/fs.h"


namespace myapp {

class FileChooserEntry : public QObject {
    Q_OBJECT

    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)

    Q_PROPERTY(QString path READ path WRITE setPath NOTIFY pathChanged)

public:
    explicit FileChooserEntry(QObject *parent = nullptr);

    explicit FileChooserEntry(const fs::path &path, QObject *parent = nullptr);

    virtual ~FileChooserEntry();

    const QString &name() const {
        return name_;
    }

    void setName(const QString &name) {
        if (name_ != name) {
            name_ = name;
            emit nameChanged();
        }
    }

    const QString &path() const {
        return path_;
    }

    void setPath(const QString &path) {
        if (path_ != path) {
            path_ = path;
            emit pathChanged();
        }
    }

signals:
    void nameChanged();

    void pathChanged();

private:
    QString name_;
    QString path_;
};

}
