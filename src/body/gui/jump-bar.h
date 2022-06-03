#pragma once

#include <QScrollBar>


namespace myapp
{

class JumpBar : public QScrollBar {
    Q_OBJECT
public:
    JumpBar(QWidget *parent = nullptr);

    virtual ~JumpBar();

protected:
    virtual void mousePressEvent(QMouseEvent *e) override;

    virtual void mouseReleaseEvent(QMouseEvent *e) override;

    virtual void mouseMoveEvent(QMouseEvent *e) override;

private:
    void jumpToY(int y);

private:
    bool jumping_ = false;
};

}
