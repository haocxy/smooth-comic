import QtQuick
import QtQuick.Controls

import "../Global"


Rectangle {
    id: idRoot
    required property string text
    required property int iconHeight

    signal clicked

    // 外部的整个矩形实际是边框，为了确保边框的内外都是有弧度的，通过两层矩形实现
    color: $engine.isWindowed && idMouseArea.containsMouse ? Style.titleBarColor.lighter(1.3) : Style.titleBarColor
    radius: 8
    antialiasing: true

    Rectangle {
        anchors.centerIn: parent
        width: parent.width - 4
        height: parent.height - 4
        radius: 6
        color: $engine.isWindowed && idMouseArea.pressed ? Style.titleBarColor.darker(1.3) : Style.titleBarColor

        Text {
            anchors.centerIn: parent
            font {
                family: "Material Icons"
                pixelSize: iconHeight
            }
            text: idRoot.text
            color: Style.iconColor
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
