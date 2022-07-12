#include "popup-layer-layout.h"

#include <QLayoutItem>
#include <QWidget>


namespace myapp {

PopupLayerLayout::PopupLayerLayout(QWidget *parent)
    : QLayout(parent)
{
}

PopupLayerLayout::~PopupLayerLayout()
{
}

void PopupLayerLayout::addItem(QLayoutItem *item)
{
    items_.push_back(uptr<QLayoutItem>(item));
}

Qt::Orientations PopupLayerLayout::expandingDirections() const
{
    return Qt::Orientation::Horizontal | Qt::Orientation::Vertical;
}

int PopupLayerLayout::count() const
{
    return static_cast<int>(items_.size());
}

QLayoutItem *PopupLayerLayout::itemAt(int index) const
{
    if (index >= 0 && index < items_.size()) {
        return items_[index].get();
    } else {
        return nullptr;
    }
}

QSize PopupLayerLayout::minimumSize() const
{
    int maxMinWidth = 0;
    int maxMinHeight = 0;

    for (const uptr<QLayoutItem> &p : items_) {
        const QSize curMinSize = p->minimumSize();
        const int curMinWidth = curMinSize.width();
        const int curMinHeight = curMinSize.height();
        if (curMinWidth > maxMinWidth) {
            maxMinWidth = curMinWidth;
        }
        if (curMinHeight > maxMinHeight) {
            maxMinHeight = curMinHeight;
        }
    }

    return QSize(maxMinWidth, maxMinHeight);
}

void PopupLayerLayout::setGeometry(const QRect &rect)
{
    QLayout::setGeometry(rect);

    for (auto &item : items_) {
        item->setGeometry(QRect(QPoint(rect.width() / 2, rect.height() - 200), item->sizeHint()));
    }
}

QSize PopupLayerLayout::sizeHint() const
{
    return QSize();
}

QLayoutItem *PopupLayerLayout::takeAt(int index)
{
    uptr<QLayoutItem> p = std::move(items_[index]);
    items_.erase(items_.begin() + index);
    return p.release();
}

}
