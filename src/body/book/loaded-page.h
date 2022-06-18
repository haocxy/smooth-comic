#pragma once

#include "core/basetype.h"


namespace myapp {

class LoadedPage {
public:
    LoadedPage() {}

    u32str name;
    i32 rawWidth{};
    i32 rawHeight{};
    scc::buff encodedRawImg;
    scc::buff encodedScaledImg;
};

}
