import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import myapp

Popup {
    required property var pageReader
    width: 600
    height: 100
    modal: true

    ButtonGroup {
        id: idButtonGroup
    }

    GridLayout {
        anchors.fill: parent
        columns: 4
        RadioButton {
            ButtonGroup.group: idButtonGroup
            text: qsTr("Raw Size")
            onClicked: pageReader.scaleMode = PageReader.ScaleMode_RawSize
        }
        RadioButton {
            ButtonGroup.group: idButtonGroup
            text: qsTr("Adjust Size")
            onClicked: pageReader.scaleMode = PageReader.ScaleMode_AutoFitAreaSize
        }
        RadioButton {
            ButtonGroup.group: idButtonGroup
            text: qsTr("Adjust Width")
            onClicked: pageReader.scaleMode = PageReader.ScaleMode_AutoFitAreaWidth
        }
        RadioButton {
            ButtonGroup.group: idButtonGroup
            text: qsTr("Adjust Height")
            onClicked: pageReader.scaleMode = PageReader.ScaleMode_AutoFitAreaHeight
        }
        RadioButton {
            ButtonGroup.group: idButtonGroup
            text: qsTr("Fix Width Ratio")
            onClicked: pageReader.scaleMode = PageReader.ScaleMode_FixWidthByRatio
        }
        RadioButton {
            ButtonGroup.group: idButtonGroup
            text: qsTr("Fix Height Ratio")
            onClicked: pageReader.scaleMode = PageReader.ScaleMode_FixHeightByRatio
        }
        RadioButton {
            ButtonGroup.group: idButtonGroup
            text: qsTr("Fix Width Pixel")
            onClicked: pageReader.scaleMode = PageReader.ScaleMode_FixWidthByPixel
        }
        RadioButton {
            ButtonGroup.group: idButtonGroup
            text: qsTr("Fix Height Pixel")
            onClicked: pageReader.scaleMode = PageReader.ScaleMode_FixHeightByPixel
        }
    }
}
