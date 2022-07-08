#pragma once


#if defined(WIN32)
#include "frameless-window.win32.h"
#else
#error "FramelessWindow unimplemented on current platform"
#endif
