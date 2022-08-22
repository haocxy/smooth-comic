import QtQuick
import QtQuick.Controls

import myapp.Global

Rectangle {
    id: idRoot
    required property string text
    signal clicked

    color: idMouseArea.containsMouse ? "#33B3FF" : GlobalStyle.titleBarColor

    Text {
        anchors.centerIn: parent
        font {
            family: "Material Icons"
            pixelSize: idRoot.height * 0.6
        }
        text: idRoot.text
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
