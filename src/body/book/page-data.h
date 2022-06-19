#pragma once

#include "core/ustr.h"
#include "core/scoped_container.h"

#include "page-num.h"


namespace myapp {

class PageData {
public:
    PageNum seqNum{};
    u32str name;
    scc::buff data;
};

}
