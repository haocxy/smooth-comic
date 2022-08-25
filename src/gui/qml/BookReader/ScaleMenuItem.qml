import QtQuick
import QtQuick.Controls

import myapp
import "../Global"

Item {
    required property ButtonGroup group
    required property var pageReader
    required property var scaleMode
    required property string text

    id: root
    implicitWidth: button.implicitWidth
    implicitHeight: button.implicitHeight

    RadioButton {
        id: button
        anchors.centerIn: parent
        background: null
        ButtonGroup.group: group
        text: root.text
        checked: pageReader ? pageReader.scaleMode === scaleMode : false
        onClicked: pageReader.scaleMode = scaleMode
    }
}
