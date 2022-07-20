#pragma once


#if defined(WIN32)
#include "frameless-window.win32.h"
#else
#include "default-frameless-window.h"

namespace myapp {
using FramelessWindow = DefaultFramelessWindow;
}
#endif
