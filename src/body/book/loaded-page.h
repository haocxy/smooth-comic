#pragma once

#include <QImage>

#include "core/basetype.h"

#include "page-num.h"


namespace myapp {

class LoadedPage {
public:
    PageNum seqNum{};
    u32str name;
    i32 rawWidth{};
    i32 rawHeight{};
    QImage rawImg;
    scc::buff encodedRawImg;
    QImage scaledImg;
    scc::buff encodedScaledImg;
};

}
