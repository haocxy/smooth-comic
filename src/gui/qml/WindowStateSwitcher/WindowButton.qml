import QtQuick
import QtQuick.Controls

import "../Global"

Rectangle {
    id: root
    required property string text
    required property color bgColor

    signal clicked

    property bool mouseOverByFramelessWindow: false
    property bool mouseOver: mouseOverByFramelessWindow || mouseArea.containsMouse

    color: bgColor

    Text {
        anchors.centerIn: parent
        font {
            family: "Material Icons"
            pixelSize: root.height * 0.6
        }
        text: root.text
        color: Style.iconColor
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
    }

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        hoverEnabled: true
        onClicked: root.clicked()
    }
}
