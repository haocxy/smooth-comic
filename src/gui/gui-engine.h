#pragma once

#include <QObject>

#include "core/declare_ptr.h"


namespace myapp {

class Engine;


class GuiEngine : public QObject {
    Q_OBJECT
public:
    explicit GuiEngine(QObject *parent = nullptr);

    virtual ~GuiEngine();

    Q_INVOKABLE void testCallFromQML(const QString &msg);

    Engine &engine();

private:
    DeclarePtr<Engine> engine_;
};

}
