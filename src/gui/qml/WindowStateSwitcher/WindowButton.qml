import QtQuick
import QtQuick.Controls

import "../Global"

Rectangle {
    id: idRoot
    required property string text
    required property color bgColor

    signal clicked

    property bool mouseOverByFramelessWindow: false
    property bool mouseOver: mouseOverByFramelessWindow || idMouseArea.containsMouse

    color: bgColor

    Text {
        anchors.centerIn: parent
        font {
            family: "Material Icons"
            pixelSize: idRoot.height * 0.6
        }
        text: idRoot.text
        color: Style.iconColor
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
    }

    MouseArea {
        id: idMouseArea
        anchors.fill: parent
        hoverEnabled: true
        onClicked: idRoot.clicked()
    }
}
