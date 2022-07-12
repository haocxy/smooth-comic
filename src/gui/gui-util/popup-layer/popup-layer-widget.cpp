#include "popup-layer-widget.h"

#include "popup-layer.h"


namespace myapp {

PopupLayerWidget::PopupLayerWidget(PopupLayer &popupLayer)
    : QFrame(&popupLayer)
    , popupLayer_(popupLayer)
{
    popupLayer.addPopup(this);
}

PopupLayerWidget::~PopupLayerWidget()
{
}

bool PopupLayerWidget::isPopupVisible() const
{
    return popupLayer_.isVisible() && isVisible();
}

void PopupLayerWidget::setPopupVisible(bool visible)
{
    popupLayer_.setVisible(visible);
    setVisible(visible);
}

}
