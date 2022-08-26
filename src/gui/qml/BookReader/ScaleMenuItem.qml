import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import myapp
import "../Global"

Item {
    required property var scaleMode
    required property string text

    id: root
    Layout.fillWidth: true
    Layout.fillHeight: true
    implicitWidth: button.implicitWidth
    implicitHeight: button.implicitHeight

    RadioButton {
        id: button
        anchors.centerIn: parent
        background: null
        ButtonGroup.group: buttons ? buttons : null
        font.pixelSize: Style.popMenuContentTextPixSize
        text: root.text
        checked: pageReader ? pageReader.scaleMode === scaleMode : false
        onClicked: pageReader.scaleMode = scaleMode
    }
}
