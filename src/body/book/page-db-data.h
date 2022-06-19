#pragma once

#include "core/ustr.h"
#include "core/basetype.h"

#include "page-num.h"


namespace myapp {

class PageDbData {
public:
    PageNum seqNum;
    u32str name;
    i32 rawWidth{};
    i32 rawHeight{};
    scc::buff rawImg;
    scc::buff scaledImg;
};

}
