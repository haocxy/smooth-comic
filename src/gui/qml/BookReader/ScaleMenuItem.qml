import QtQuick
import QtQuick.Controls

import myapp
import "../Global"

Item {
    required property var menu
    required property var scaleMode
    required property string text

    id: root
    implicitWidth: button.implicitWidth
    implicitHeight: button.implicitHeight

    RadioButton {
        id: button
        anchors.centerIn: parent
        background: null
        ButtonGroup.group: menu.buttons ? menu.buttons : null
        text: root.text
        checked: menu.pageReader ? menu.pageReader.scaleMode === scaleMode : false
        onClicked: menu.pageReader.scaleMode = scaleMode
    }
}
