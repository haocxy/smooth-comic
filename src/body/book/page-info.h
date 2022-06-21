#pragma once

#include "core/ustr.h"
#include "core/basetype.h"

#include "page-num.h"


namespace myapp {

class PageInfo {
public:
    PageNum seqNum{};
    u32str name;
    i32 rawWidth{};
    i32 rawHeight{};
};

}
