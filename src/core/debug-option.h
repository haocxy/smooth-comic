#pragma once

#include "debug-option/debug-option.h"
#include "debug-option/ui-path.h"


namespace myapp {

template <typename T>
using DebugOption = debug_option::DebugOption<T>;

using DebugPath = debug_option::UiPath;

using DebugPathOption = debug_option::DebugOption<debug_option::UiPath>;

}
