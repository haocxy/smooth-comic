#pragma once

#include <vector>
#include <deque>

#include <QObject>
#include <QString>
#include <QList>

#include <QQmlListProperty>

#include "core/basetype.h"
#include "core/fs.h"

#include "file-chooser-stack.h"


namespace myapp {

class FileChooser : public QObject {
    Q_OBJECT

    Q_PROPERTY(FileChooserStack *stack READ stack NOTIFY stackChanged)

public:
    explicit FileChooser(QObject *parent = nullptr);

    virtual ~FileChooser();

    Q_INVOKABLE void openInitDir();

    Q_INVOKABLE void openDir(const QString &path);

    Q_INVOKABLE void goBack();

    FileChooserStack *stack() {
        return &stack_;
    }

signals:
    void stackChanged();

private:
    FileChooserStack stack_;
};

}
