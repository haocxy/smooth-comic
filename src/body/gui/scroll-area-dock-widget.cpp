#include "scroll-area-dock-widget.h"

#include "thumb-list-scroll-area.h"


namespace myapp {

ScrollAreaDockWidget::ScrollAreaDockWidget(QScrollArea *scrollArea, const QString &title, QWidget *parent)
    : QDockWidget(title, parent)
{
    scrollArea->setParent(this);

    setWidget(scrollArea);

    connect(this, &ScrollAreaDockWidget::topLevelChanged, this, [this, scrollArea](bool topLevel) {
        const QSize oldSize = size();
        scrollArea->widget()->adjustSize();
        resize(oldSize);
    });
}

ScrollAreaDockWidget::~ScrollAreaDockWidget()
{

}

}
