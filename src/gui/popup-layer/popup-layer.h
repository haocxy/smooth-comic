#pragma once

#include <QWidget>


namespace myapp {

class PopupLayer : public QWidget {
    Q_OBJECT
public:
    explicit PopupLayer(QWidget *parent = nullptr);

    virtual ~PopupLayer();
};

}
