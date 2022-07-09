#pragma once

#include <QWidget>
#include <QPointer>
#include <QBoxLayout>
#include <QLabel>

#include "gui-util/font-icon.h"


namespace myapp {

class ControllButton;

class ControllItem : public QWidget {
    Q_OBJECT
public:
    explicit ControllItem(const FontIcon &icon, const QString &text, QWidget *parent = nullptr);

    virtual ~ControllItem();

    void setIcon(const FontIcon &icon);

    void setShowLabel(bool showLabel);

signals:
    void clicked();

private:
    QPointer<QVBoxLayout> layout_;
    QPointer<ControllButton> button_;
    QPointer<QLabel> label_;
};

}
