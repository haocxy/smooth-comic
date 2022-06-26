#pragma once

#include <QWidget>


namespace myapp {

class PageControllLayer : public QWidget {
    Q_OBJECT
public:
    explicit PageControllLayer(QWidget *parent = nullptr);

    virtual ~PageControllLayer();
};

}
