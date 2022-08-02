#pragma once

#include <QObject>


namespace myapp {

class GuiEngine : public QObject {
    Q_OBJECT
public:
    explicit GuiEngine(QObject *parent = nullptr);

    virtual ~GuiEngine();
};

}
