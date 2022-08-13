#pragma once

#include <QObject>


namespace myapp {

class FileChooserStackInfo : public QObject {
    Q_OBJECT
public:
    explicit FileChooserStackInfo(QObject *parent = nullptr);

    virtual ~FileChooserStackInfo();
};

}
