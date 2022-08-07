#pragma once

#include <QObject>


namespace myapp {

class FileTree : public QObject {
    Q_OBJECT
public:
    explicit FileTree(QObject *parent = nullptr);

    virtual ~FileTree();
};

}
