#pragma once

#include <QFrame>

#include "gui-util/popup.h"

#include "core/declare_ptr.h"


namespace Ui {
class ScaleSetting;
}

namespace myapp {

class ScaleSettingPopup : public Popup {
    Q_OBJECT
public:
    explicit ScaleSettingPopup(QWidget *parent = nullptr);

    virtual ~ScaleSettingPopup();

private:
    QPointer<QFrame> rootWidget_;
    DeclarePtr<Ui::ScaleSetting> ui_;
};

}
