#pragma once


namespace myapp {

enum class FontIconEnum : char16_t {

    Remove = 0xe15b,
    FolderOpen = 0xe2c8,
    FilterNone = 0xe3e0,
    NavigateBefore = 0xe408,
    NavigateNext = 0xe409,
    RotateRight = 0xe41a,
    Close = 0xe5cd,
    Fullscreen = 0xe5d0,
    Menu = 0xe5d2,
    MoreVert = 0xe5d4,
    AutoStories = 0xe666,
    CheckBoxOutlineBlank = 0xe835,
    Search = 0xe8b6,
    ZoomIn = 0xe8ff,
    ZoomOut = 0xe900,
    OpenInFull = 0xf1ce,
    CloseFullscreen = 0xf1cf,

    WindowFullScreen = OpenInFull,
    WindowBackToWindow = CloseFullscreen,
    WindowMinimize = Remove,
    WindowMaximize = CheckBoxOutlineBlank,
    WindowRestore = FilterNone,
    WindowClose = Close,

    NavigateLeft = NavigateBefore,
    NavigateRight = NavigateNext,
};

}
