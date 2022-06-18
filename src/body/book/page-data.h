#pragma once

#include "core/ustr.h"
#include "core/scoped_container.h"


namespace myapp {

class PageData {
public:
    u32str name;
    scc::buff data;
};

}
