#pragma once

#include <vector>

#include <QLayout>

#include "core/basetype.h"


namespace myapp {

class PopupLayerLayout : public QLayout {
public:
    explicit PopupLayerLayout(QWidget *parent = nullptr);

    virtual ~PopupLayerLayout();

    virtual void addItem(QLayoutItem *item) override;

    virtual Qt::Orientations expandingDirections() const override;

    virtual int count() const override;

    virtual QLayoutItem *itemAt(int index) const override;

    virtual QSize minimumSize() const override;

    virtual void setGeometry(const QRect &rect) override;

    virtual QSize sizeHint() const override;

    virtual QLayoutItem *takeAt(int index) override;

private:
    std::vector<uptr<QLayoutItem>> items_;
};

}
