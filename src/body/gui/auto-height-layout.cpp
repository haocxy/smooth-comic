#include "auto-height-layout.h"


namespace myapp {

AutoHeightLayout::AutoHeightLayout(QWidget *parent)
    : QLayout(parent)
{
}

AutoHeightLayout::~AutoHeightLayout()
{
    QLayoutItem *item;
    while ((item = takeAt(0)))
        delete item;
}

void AutoHeightLayout::addItem(QLayoutItem *item)
{
    items_.append(item);
}

int AutoHeightLayout::count() const
{
    return items_.size();
}

QLayoutItem *AutoHeightLayout::itemAt(int index) const
{
    return items_.value(index);
}

QLayoutItem *AutoHeightLayout::takeAt(int index)
{
    if (index >= 0 && index < items_.size()) {
        return items_.takeAt(index);
    }

    return nullptr;
}

Qt::Orientations AutoHeightLayout::expandingDirections() const
{
    return {};
}

bool AutoHeightLayout::hasHeightForWidth() const
{
    return true;
}

int AutoHeightLayout::heightForWidth(int width) const
{
    return walk(QPoint(0, 0), width, WalkType::Calc);
}

void AutoHeightLayout::setGeometry(const QRect &rect)
{
    QLayout::setGeometry(rect);
    walk(rect.topLeft(), rect.width(), WalkType::Layout);
}

QSize AutoHeightLayout::sizeHint() const
{
    return minimumSize();
}

QSize AutoHeightLayout::minimumSize() const
{
    return {};
}

int AutoHeightLayout::walk(const QPoint &base, int totalWidth, WalkType walkType) const
{
    const Margins m = getMargins();

    const int spaceY = std::min(m.top, m.bottom);

    const int x = base.x() + m.left;
    const int initY = base.y() + m.top;
    int y = initY;

    const int contentWidth = totalWidth - m.left - m.right;

    for (QLayoutItem *item : items_) {

        const QSize itemSize{ contentWidth, item->heightForWidth(contentWidth) };

        if (walkType == WalkType::Layout) {
            item->setGeometry(QRect(QPoint(x, y), itemSize));
        }

        y += itemSize.height() + spaceY;
    }

    return y  + m.bottom - base.y();
}

}
