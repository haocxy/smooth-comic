#pragma once

#include <QObject>


namespace myapp {

class FileTreeEntry : public QObject {
    Q_OBJECT

    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
public:
    explicit FileTreeEntry(QObject *parent = nullptr);

    explicit FileTreeEntry(const QString &name, QObject *parent = nullptr);

    virtual ~FileTreeEntry();

    const QString &name() const {
        return name_;
    }

    void setName(const QString &name) {
        if (name_ != name) {
            name_ = name;
            nameChanged();
        }
    }

signals:
    void nameChanged();

private:
    QString name_;
};

}
