#pragma once

#include "core/basetype.h"
#include "core/ustr.h"

#include "page-num.h"


namespace myapp {

class PageData {
public:
    PageNum seqNum{};
    u32str name;
    Buff data;
};

}
