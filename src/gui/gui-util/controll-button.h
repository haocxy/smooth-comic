#pragma once

#include <QPushButton>

#include "gui-util/font-icon-enum.h"


namespace myapp {

class ControllButton : public QPushButton {
    Q_OBJECT
public:
    explicit ControllButton(FontIconEnum icon, QWidget *parent = nullptr);

    virtual ~ControllButton();
};

}
