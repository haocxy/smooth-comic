import QtQuick
import QtQuick.Controls

import myapp.Global

Rectangle {
    id: idRoot
    required property string text
    signal clicked

    property bool mouseOverByFramelessWindow: false

    property bool mouseOver: mouseOverByFramelessWindow || idMouseArea.containsMouse

    color: mouseOver ? GlobalStyle.titleBarColor.lighter(1.3) : GlobalStyle.titleBarColor

    Text {
        anchors.centerIn: parent
        font {
            family: "Material Icons"
            pixelSize: idRoot.height * 0.6
        }
        text: idRoot.text
        color: GlobalStyle.iconColor
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
