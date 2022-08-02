#pragma once

#include <QObject>


namespace myapp {

class GuiBookManager : public QObject {
    Q_OBJECT
public:
    explicit GuiBookManager(QObject *parent = nullptr);

    virtual ~GuiBookManager();
};

}
