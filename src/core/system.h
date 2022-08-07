#pragma once

#include <cstddef>
#include <vector>

#include "fs.h"
#include "basetype.h"


enum class PlatformType {
    Unknown,
    Windows,
    Android,
};

namespace SystemUtil
{

#if defined(WIN32)
constexpr PlatformType platformType = PlatformType::Windows;
#elif defined(ANDROID)
constexpr PlatformType platformType = PlatformType::Android;
#else
constexpr PlatformType platformType = PlatformType::Unknown;
#endif

i32 pageSize();

i32 processorCount();

fs::path userHome();

fs::path exePath();

fs::path defaultOpenFileDir();

}
