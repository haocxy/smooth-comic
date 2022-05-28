#pragma once

#include <QObject>


namespace myapp {


class Book : public QObject {
    Q_OBJECT
public:
    explicit Book(QObject *parent = nullptr);

};


}
