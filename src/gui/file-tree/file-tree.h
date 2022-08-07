#pragma once

#include <vector>

#include <QObject>
#include <QString>


namespace myapp {

class FileTree : public QObject {
    Q_OBJECT

    Q_PROPERTY(QString currDir READ currDir WRITE setCurrDir NOTIFY currDirChanged)
public:
    explicit FileTree(QObject *parent = nullptr);

    virtual ~FileTree();

    const QString &currDir() const {
        return currDir_;
    }

    void setCurrDir(const QString &dir);

signals:
    void currDirChanged();

private:
    QString currDir_;

    std::vector<QString> entries_;
};

}
