#pragma once

#include <QLayout>


namespace myapp {

class PopupLayerLayout : public QLayout {
public:
    explicit PopupLayerLayout(QWidget *parent = nullptr);

    virtual ~PopupLayerLayout();
};

}
