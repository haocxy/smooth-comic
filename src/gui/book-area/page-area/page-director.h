#pragma once

#include <QObject>


namespace myapp {

class PageDirector : public QObject {
    Q_OBJECT
public:
    explicit PageDirector(QObject *parent = nullptr);

    virtual ~PageDirector();
};

}
