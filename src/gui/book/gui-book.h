#pragma once

#include <QObject>


namespace myapp {

class GuiBook : public QObject {
    Q_OBJECT
public:
    explicit GuiBook(QObject *parent = nullptr);

    virtual ~GuiBook();
};

}
