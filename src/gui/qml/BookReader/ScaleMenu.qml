import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import myapp

Popup {
    required property var pageReader
    id: root
    width: 320
    height: 400
    modal: true

    ButtonGroup {
        id: buttons
    }

    ColumnLayout {
        anchors.fill: parent
        id: rootLayout
        Text {
            Layout.fillWidth: true; Layout.fillHeight: true
            text: qsTr("Auto Scale")
        }
        GridLayout {
            Layout.fillWidth: true; Layout.fillHeight: true
            columns: 2
            ScaleMenuItem {
                Layout.fillWidth: true; Layout.fillHeight: true
                group: buttons
                pageReader: root.pageReader
                scaleMode: PageReader.ScaleMode_RawSize
                text: qsTr("Raw Size")
            }
            ScaleMenuItem {
                Layout.fillWidth: true; Layout.fillHeight: true
                group: buttons
                pageReader: root.pageReader
                scaleMode: PageReader.ScaleMode_AutoFitAreaSize
                text: qsTr("Adjust Size")
            }
            ScaleMenuItem {
                Layout.fillWidth: true; Layout.fillHeight: true
                group: buttons
                pageReader: root.pageReader
                scaleMode: PageReader.ScaleMode_AutoFitAreaWidth
                text: qsTr("Adjust Width")
            }
            ScaleMenuItem {
                Layout.fillWidth: true; Layout.fillHeight: true
                group: buttons
                pageReader: root.pageReader
                scaleMode: PageReader.ScaleMode_AutoFitAreaHeight
                text: qsTr("Adjust Height")
            }
        }
        Text {
            Layout.fillWidth: true; Layout.fillHeight: true
            text: qsTr("Fix Scale")
        }
        GridLayout {
            Layout.fillWidth: true; Layout.fillHeight: true
            columns: 2
            ScaleMenuItem {
                Layout.fillWidth: true; Layout.fillHeight: true
                group: buttons
                pageReader: root.pageReader
                scaleMode: PageReader.ScaleMode_FixWidthByRatio
                text: qsTr("Fix Width Ratio")
            }
            ScaleMenuItem {
                Layout.fillWidth: true; Layout.fillHeight: true
                group: buttons
                pageReader: root.pageReader
                scaleMode: PageReader.ScaleMode_FixHeightByRatio
                text: qsTr("Fix Height Ratio")
            }
            ScaleMenuItem {
                Layout.fillWidth: true; Layout.fillHeight: true
                group: buttons
                pageReader: root.pageReader
                scaleMode: PageReader.ScaleMode_FixWidthByPixel
                text: qsTr("Fix Width Pixel")
            }
            ScaleMenuItem {
                Layout.fillWidth: true; Layout.fillHeight: true
                group: buttons
                pageReader: root.pageReader
                scaleMode: PageReader.ScaleMode_FixHeightByPixel
                text: qsTr("Fix Height Pixel")
            }
        }
    }
}
