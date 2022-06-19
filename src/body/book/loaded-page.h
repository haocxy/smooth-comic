#pragma once

#include "core/basetype.h"

#include "page-num.h"


namespace myapp {

class LoadedPage {
public:
    PageNum seqNum{};
    u32str name;
    i32 rawWidth{};
    i32 rawHeight{};
    scc::buff encodedRawImg;
    scc::buff encodedScaledImg;
};

}
