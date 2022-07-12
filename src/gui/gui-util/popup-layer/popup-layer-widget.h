#pragma once

#include <QWidget>


namespace myapp {

class PopupLayerWidget : public QWidget {
    Q_OBJECT
public:
    explicit PopupLayerWidget(QWidget *parent = nullptr);

    virtual ~PopupLayerWidget();
};

}
