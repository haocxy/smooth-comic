#pragma once

#include <QObject>

#include "core/declare_ptr.h"


namespace myapp {

class Engine;


class GuiEngine : public QObject {
    Q_OBJECT

    Q_PROPERTY(bool isWindowed READ isWindowed NOTIFY isWindowedChanged)
public:
    explicit GuiEngine(QObject *parent = nullptr);

    virtual ~GuiEngine();

    Q_INVOKABLE void testCallFromQML(const QString &msg);

    Engine &engine();

    bool isWindowed() const {
        return isWindowed_;
    }

signals:
    void isWindowedChanged();

    void keyBackReleased();

private:
    DeclarePtr<Engine> engine_;

    bool isWindowed_{ false };
};

}
