import QtQuick
import QtQuick.Controls

import myapp.Global


Rectangle {
    id: idRoot
    required property string text
    required property int iconHeight

    signal clicked

    // 外部的整个矩形实际是边框，为了确保边框的内外都是有弧度的，通过两层矩形实现
    color: idMouseArea.containsMouse ? GlobalStyle.titleBarColor.lighter(1.3) : GlobalStyle.titleBarColor
    radius: 8
    antialiasing: true

    Rectangle {
        anchors.centerIn: parent
        width: parent.width - 4
        height: parent.height - 4
        radius: 6
        color: idMouseArea.pressed ? GlobalStyle.titleBarColor.darker(1.3) : GlobalStyle.titleBarColor

        Text {
            anchors.centerIn: parent
            font {
                family: "Material Icons"
                pixelSize: iconHeight
            }
            text: idRoot.text
            color: GlobalStyle.iconColor
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
        }
    }

    MouseArea {
        id: idMouseArea
        anchors.fill: parent
        hoverEnabled: true
        onClicked: idRoot.clicked()
    }
}
