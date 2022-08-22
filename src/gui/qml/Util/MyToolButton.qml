import QtQuick
import QtQuick.Controls

Rectangle {
    id: idRoot
    required property string text
    signal clicked

    // 外部的整个矩形实际是边框，为了确保边框的内外都是有弧度的，通过两层矩形实现
    color: idMouseArea.containsMouse ? "#33B3FF" : "#FFFFFF"
    radius: 8
    antialiasing: true

    Rectangle {
        anchors.centerIn: parent
        width: parent.width - 4
        height: parent.height - 4
        radius: 6
        color: idMouseArea.pressed ? "#E4E4E4" : "#FFFFFF"

        Text {
            anchors.centerIn: parent
            font {
                family: "Material Icons"
                pixelSize: 26
            }
            text: idRoot.text
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
