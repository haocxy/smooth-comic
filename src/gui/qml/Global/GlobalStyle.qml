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

    property color readerBgColor4ControllState: "white"
    property color readerBgColor4ReadState: "black"

    // popup菜单中区域标题中的文本像素尺寸
    property int popMenuTitleTextPixSize: 24
    property int popMenuTitleAreaHeight: 40
    property int popMenuContentMargin: 16
    property int popMenuContentRowHeight: 24
    property int popMenuSegGap: 8
    property color popMenuBgColor: "#eaeaea"
}
