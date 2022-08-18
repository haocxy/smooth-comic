#pragma once

#include <QObject>

#include "core/basetype.h"
#include "core/fs.h"


namespace myapp {

class FileChooserStackInfo : public QObject {
    Q_OBJECT
public:
    explicit FileChooserStackInfo(const fs::path &dir, qreal contentY, QObject *parent = nullptr);

    virtual ~FileChooserStackInfo();

    const fs::path &dir() const {
        return dir_;
    }

    qreal contentY() const {
        return contentY_;
    }

private:
    fs::path dir_;
    qreal contentY_{};
};

}
