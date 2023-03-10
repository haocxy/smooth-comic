#pragma once

#include <QWidget>
#include <QScrollArea>
#include <QPointer>
#include <QBoxLayout>

#include "core/basetype.h"

#include "gui-util/jump-bar.h"
#include "thumb-list.h"


namespace myapp {

class Controller;


class ThumbArea : public QScrollArea {
    Q_OBJECT
public:
    explicit ThumbArea(Controller &controller, QWidget *parent = nullptr);

    virtual ~ThumbArea() {}

    ThumbList *thumbList() {
        return root_;
    }

protected:
    virtual void resizeEvent(QResizeEvent *) override;

private:
    Controller &controller_;

private:
    QPointer<JumpBar> jumpBar_;
    QPointer<ThumbList> root_;

    opt<double> scrollPercent_;
};


}
