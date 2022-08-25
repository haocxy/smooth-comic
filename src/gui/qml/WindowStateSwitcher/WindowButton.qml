import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import "../Global"

Rectangle {
    id: root
    required property string text

    property bool mouseOverByFramelessWindow: false
    property bool mouseOver: mouseOverByFramelessWindow || mouseArea.containsMouse
    property color bgColor: mouseOver ? hoverColor : btnBgColor

    Layout.topMargin: btnVMargin
    Layout.bottomMargin: btnVMargin
    Layout.leftMargin: btnHMargin
    Layout.rightMargin: btnHMargin
    Layout.fillHeight: true
    Layout.preferredWidth: height * heightFactor
    color: bgColor

    signal clicked

    Text {
        anchors.centerIn: parent
        font {
            family: FontIcon.fontFamily
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
