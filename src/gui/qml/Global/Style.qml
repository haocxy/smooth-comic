pragma Singleton

import QtQuick

QtObject {
    property int titleBarHeight: 32
    property color titleBarColor: "#33B3FF"
    property color titleBarColor4ReadState: "#3f3f3f"

    property int topBarHeight: 64
    property int topBarIconHeight: 32

    property int bottomBarHeight: 64
    property int bottomBarIconHeight: 32

    property color iconColor: "white"

    property color readerBgColor4ControllState: "#f7f5f5"
    property color readerBgColor4ReadState: "black"

    property real readControlAreaBgOpacity: 0.9

    // popup菜单中区域标题中的文本像素尺寸
    property color popMenuTitleBarBgColor: titleBarColor
    property color popMenuTitleBarIconColor: iconColor
    property int popMenuTitleBarHeight: 40
    property real popMenuTitleButtonHeightRatio: 0.6
    property int popMenuContentMargin: 8
    property int popMenuContentRowHeight: 20
    property int popMenuSegGap: 8
    property color popMenuBgColor: "#eaeaea"
    property real popMenuOpacity: 1
    property int popMenuRadius: 8
    property int popMenuMargin: 8
    property int popMenuContentTextPixSize: 14
}
