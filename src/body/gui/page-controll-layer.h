#pragma once

#include <QWidget>

#include "core/basetype.h"

#include "switch-direction.h"


namespace myapp {

class PageControllLayer : public QWidget {
    Q_OBJECT
public:
    explicit PageControllLayer(QWidget *parent = nullptr);

    virtual ~PageControllLayer();

    void setShowControllAreas(bool showControllAreas) {
        if (showControllAreas_ != showControllAreas) {
            showControllAreas_ = showControllAreas;
            update();
        }
    }

    void toggleShowControllAreas() {
        showControllAreas_ = !showControllAreas_;
        update();
    }

    class AreaConfig {
    public:
        AreaConfig() {}

        void setTotalArea(const QSize &size) {
            totalArea_ = size;
            clearCache();
        }

        const QRect &leftSwitcher() const {
            if (!leftSwitcher_) {
                const int width = totalArea_.width();
                const int height = totalArea_.height();
                const int x = 0;
                const int y = height * switchTop_;
                const int w = width * switchLeft_;
                const int h = height * (switchBottom_ - switchTop_);
                leftSwitcher_ = QRect(x, y, w, h);
            }
            return *leftSwitcher_;
        }

        const QRect &rightSwitcher() const {
            if (!rightSwitcher_) {
                const int width = totalArea_.width();
                const int height = totalArea_.height();
                const int x = width * switchRight_;
                const int y = height * switchTop_;
                const int w = width - x;
                const int h = height * (switchBottom_ - switchTop_);
                rightSwitcher_ = QRect(x, y, w, h);
            }
            return *rightSwitcher_;
        }

    private:
        void clearCache() {
            leftSwitcher_.reset();
            rightSwitcher_.reset();
        }

    private:
        QSize totalArea_;
        float switchLeft_ = 0.2f;
        float switchRight_ = 0.8f;
        float switchTop_ = 0.2f;
        float switchBottom_ = 0.8f;


        mutable opt<QRect> leftSwitcher_;
        mutable opt<QRect> rightSwitcher_;
    };

signals:
    void sigCmdSwitchNextPage(SwitchDirection direction);

protected:
    virtual void resizeEvent(QResizeEvent *e) override;

    virtual void paintEvent(QPaintEvent *e) override;

    virtual void mouseReleaseEvent(QMouseEvent *e) override;

private:
    AreaConfig areaConfig_;
    bool showControllAreas_{ false };
};

}
