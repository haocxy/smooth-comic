#pragma once

#include <QPainter>


namespace myapp {

class PainterSaver {
public:
    PainterSaver(QPainter &painter)
        : painter_(painter) {

        painter_.save();
    }

    ~PainterSaver() {
        painter_.restore();
    }

private:
    QPainter &painter_;
};

}
