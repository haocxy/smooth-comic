#pragma once

#include <QWidget>


namespace myapp {

class ArtboardWidget : public QWidget {
    Q_OBJECT
public:
    explicit ArtboardWidget(QWidget *parent = nullptr);

protected:
    virtual void paintEvent(QPaintEvent *) override;
};

}
