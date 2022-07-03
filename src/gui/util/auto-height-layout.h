#pragma once

#include <QLayout>


namespace myapp {

class AutoHeightLayout : public QLayout
{
public:
    explicit AutoHeightLayout(QWidget *parent = nullptr);

    virtual ~AutoHeightLayout();

    virtual void addItem(QLayoutItem *item) override;

    virtual Qt::Orientations expandingDirections() const override;

    virtual bool hasHeightForWidth() const override;

    virtual int heightForWidth(int) const override;

    virtual int count() const override;

    virtual QLayoutItem *itemAt(int index) const override;

    virtual QSize minimumSize() const override;

    virtual void setGeometry(const QRect &rect) override;

    virtual QSize sizeHint() const override;

    virtual QLayoutItem *takeAt(int index) override;

private:
    enum class WalkType {
        Calc, Layout
    };

    int walk(const QPoint &base, int totalWidth, WalkType walkType) const;

    struct Margins {
        Margins(int top, int right, int bottom, int left)
            : top(top), right(right), bottom(bottom), left(left) {}

        int top{};
        int right{};
        int bottom{};
        int left{};
    };

    Margins getMargins() const {
        int left = 0;
        int top = 0;
        int right = 0;
        int bottom = 0;
        getContentsMargins(&left, &top, &right, &bottom);
        return Margins(top, right, bottom, left);
    }

private:
    QList<QLayoutItem *> items_;
};

}
