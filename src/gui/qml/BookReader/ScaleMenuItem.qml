import QtQuick
import QtQuick.Controls

import myapp
import myapp.Global

RadioButton {
    required property ButtonGroup group
    required property var pageReader
    required property var scaleMode
    id: button
    background: Rectangle {
        color: GlobalStyle.popMenuBgColor
        opacity: 0
    }
    ButtonGroup.group: group
    checked: pageReader ? pageReader.scaleMode === scaleMode : false
    onClicked: pageReader.scaleMode = scaleMode
}
