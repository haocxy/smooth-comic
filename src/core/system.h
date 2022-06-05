#pragma once

#include <cstddef>
#include <vector>

#include "fs.h"
#include "basetype.h"


namespace SystemType
{

#ifdef WIN32
constexpr bool IsWindows = true;
#else
constexpr bool IsWindows = false;
#endif

// 确保 IsWindows 是编译期的常量
static_assert(IsWindows || !IsWindows);

}

namespace SystemUtil
{

i32 pageSize();

i32 processorCount();

fs::path userHome();

fs::path exePath();

}
