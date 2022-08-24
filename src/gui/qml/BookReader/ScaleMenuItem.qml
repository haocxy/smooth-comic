import QtQuick
import QtQuick.Controls

import myapp
import myapp.Global

Rectangle {
    required property ButtonGroup group
    required property var pageReader
    required property var scaleMode
    required property string text

    id: root
    implicitWidth: button.implicitWidth
    implicitHeight: button.implicitHeight
    color: GlobalStyle.popMenuBgColor

    RadioButton {
        id: button
        anchors.verticalCenter: parent.verticalCenter
        background: Rectangle { color: GlobalStyle.popMenuBgColor }
        ButtonGroup.group: group
        text: root.text
        checked: pageReader ? pageReader.scaleMode === scaleMode : false
        onClicked: pageReader.scaleMode = scaleMode
    }
}
