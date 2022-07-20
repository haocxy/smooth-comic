#include "popup-layer-layout.h"

#include <sstream>

#include <QLayoutItem>
#include <QWidget>
#include <QMargins>

#include "popup-layer-widget.h"


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
    QWidget *widget = item->widget();
    if (widget && dynamic_cast<PopupLayerWidget *>(widget)) {
        items_.push_back(uptr<QLayoutItem>(item));
    } else {
        throw std::logic_error("PopupLayerLayout can only add PopupLayerWidget");
    }
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

    const QMargins m = contentsMargins();

    return QSize(
        maxMinWidth + m.left() + m.right(),
        maxMinHeight + m.top() + m.bottom()
    );
}

QRect PopupLayerLayout::calcGeometryForPinToWidget(const QRect &rect, const PopupLayerWidget &popup) const
{
    const QWidget *target = popup.pinTarget();

    const QSize sizeHint = popup.sizeHint();

    const int rw = rect.width();
    const int rh = rect.height();

    const int w = sizeHint.width();
    const int h = sizeHint.height();

    {
        if (!target) [[unlikely]] {
            const int x = (rw - w) / 2;
            const int y = (rh - h) / 2;
            return QRect(x, y, sizeHint.width(), sizeHint.height());
        }
    }

    const QWidget *layer = parentWidget();

    // step 1: 根据目标控件找到目标点坐标
    const QPoint globalPinPos = target->mapToGlobal(QPoint(target->width() / 2, 0));

    // step 2: 根据目标点坐标找到弹出控件的基准点坐标
    const QPoint globalPopupBasePos = QPoint(globalPinPos.x(), globalPinPos.y() - popup.pinPadding());

    // step 3: 根据弹出控件的基准点坐标找到弹出控件的坐标
    const QPoint globalPopupPos = QPoint(globalPopupBasePos.x() - w / 2, globalPopupBasePos.y() - h);

    QPoint localPopupPos = layer->mapFromGlobal(globalPopupPos);

    if (localPopupPos.x() < contentsMargins().left()) [[unlikely]] {
        localPopupPos.setX(contentsMargins().left());
    }

    const int xLimit = rw - w - contentsMargins().right();
    if (localPopupPos.x() > xLimit) [[unlikely]] {
        localPopupPos.setX(xLimit);
    }

    if (localPopupPos.y() < contentsMargins().top()) [[unlikely]] {
        localPopupPos.setY(contentsMargins().top());
    }

    const int yLimit = rh - h - contentsMargins().bottom();
    if (localPopupPos.y() > yLimit) [[unlikely]] {
        localPopupPos.setY(yLimit);
    }

    return QRect(rect.topLeft() + localPopupPos, sizeHint);
}

QRect PopupLayerLayout::calcGeometry(const QRect &rect, const PopupLayerWidget &popup) const
{
    const std::optional<PopupLayerWidget::PinType> pinType = popup.pinType();
    if (pinType) {
        switch (*pinType) {
        case PopupLayerWidget::PinType::ToWidget:
            return calcGeometryForPinToWidget(rect, popup);
        default:
            throw std::logic_error("PopupLayerLayout: bad pinType");
        }
    } else {
        std::ostringstream ss;
        ss << "PopupLayerLayout: pinType unspecified for "
            << popup.metaObject()->className();
        throw std::logic_error(ss.str());
    }
}

void PopupLayerLayout::setGeometry(const QRect &rect)
{
    QLayout::setGeometry(rect);

    for (auto &item : items_) {
        PopupLayerWidget *popup = static_cast<PopupLayerWidget *>(item->widget());
        const QRect geometry = calcGeometry(rect, *popup);
        item->setGeometry(geometry);
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
