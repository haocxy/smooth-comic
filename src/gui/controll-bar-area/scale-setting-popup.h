#pragma once

#include <QFrame>

#include "core/declare_ptr.h"


namespace Ui {
class ScaleSetting;
}

namespace myapp {

class ScaleSettingPopup : public QFrame {
    Q_OBJECT
public:
    explicit ScaleSettingPopup(QWidget *parent = nullptr);

    virtual ~ScaleSettingPopup();

private:
    DeclarePtr<Ui::ScaleSetting> ui_;
};

}
