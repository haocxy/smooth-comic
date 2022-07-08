#pragma once


namespace myapp {

enum class FontIconEnum : char16_t {
    Refresh = 0xe72c,

    ChevronLeft = 0xe76b,
    SwitchLeft = ChevronLeft,

    ChevronRight = 0xe76c,
    SwitchRight = ChevronRight,

    ChromeClose = 0xe8bb,
    WindowClose = ChromeClose,

    OpenFile = 0xe8e5,

    ChromeMinimize = 0xe921,
    WindowMinimize = ChromeMinimize,

    ChromeMaximize = 0xe922,
    WindowMaximize = ChromeMaximize,

    ChromeRestore = 0xe923,
    WindowRestore = ChromeRestore,

    ChromeBackToWindow = 0xe92c,
    WindowBackToWindow = ChromeBackToWindow,

    ChromeFullScreen = 0xe92d,
    WindowFullScreen = ChromeFullScreen,

    ArrowLeft8 = 0xf0b0,
    


};

}
