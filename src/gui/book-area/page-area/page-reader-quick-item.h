#pragma once

#include <QQuickPaintedItem>
#include <QPointer>

#include "core/declare_ptr.h"

#include "gui/controller/controller.h"


namespace myapp {

class Controller;

class PageScene;


class PageReaderQuickItem : public QQuickPaintedItem {
    Q_OBJECT

    Q_PROPERTY(Controller *controller READ controller WRITE setController NOTIFY controllerChanged)

    Q_PROPERTY(ScaleMode scaleMode READ scaleMode WRITE setScaleMode NOTIFY scaleModeChanged)

public:
    explicit PageReaderQuickItem(QQuickItem *parent = nullptr);

    virtual ~PageReaderQuickItem();

    virtual void paint(QPainter *painter) override;

    enum ScaleMode : ScaleModeBaseType {
        ScaleMode_RawSize = ScaleModeBaseType(myapp::ScaleMode::RawSize),
        ScaleMode_AutoFitAreaSize = ScaleModeBaseType(myapp::ScaleMode::AutoFitAreaSize),
        ScaleMode_AutoFitAreaWidth = ScaleModeBaseType(myapp::ScaleMode::AutoFitAreaWidth),
        ScaleMode_AutoFitAreaHeight = ScaleModeBaseType(myapp::ScaleMode::AutoFitAreaHeight),
        ScaleMode_FixWidthByRatio = ScaleModeBaseType(myapp::ScaleMode::FixWidthByRatio),
        ScaleMode_FixHeightByRatio = ScaleModeBaseType(myapp::ScaleMode::FixHeightByRatio),
        ScaleMode_FixWidthByPixel = ScaleModeBaseType(myapp::ScaleMode::FixWidthByPixel),
        ScaleMode_FixHeightByPixel = ScaleModeBaseType(myapp::ScaleMode::FixHeightByPixel)
    };
    Q_ENUM(ScaleMode);

    ScaleMode scaleMode() const;

    void setScaleMode(ScaleMode scaleMode);

    Controller *controller() {
        return controller_;
    }

    void setController(Controller *controller);

signals:
    void controllerChanged();

    void scaleModeChanged();

private:
    void updateSceneSize();

private:
    QPointer<Controller> controller_;
    DeclarePtr<PageScene> scene_;
};

}
