#pragma once


namespace myapp {

enum class FontIconEnum : char16_t {
    ChromeBackToWindow = 0xe92c,
    WindowBackToWindow = ChromeBackToWindow,

    ChromeFullScreen = 0xe92d,
    WindowFullScreen = ChromeFullScreen,

    ChromeMinimize = 0xe921,
    WindowMinimize = ChromeMinimize,

    ChromeMaximize = 0xe922,
    WindowMaximize = ChromeMaximize,

    ChromeRestore = 0xe923,
    WindowRestore = ChromeRestore,

    ChromeClose = 0xe8bb,
    WindowClose = ChromeClose,
};

}
