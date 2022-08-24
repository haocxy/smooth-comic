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
        Text {
            text: qsTr("Auto Scale")
        }
        GridLayout {
            columns: 2
            ScaleMenuItem {
                group: buttons
                pageReader: root.pageReader
                scaleMode: PageReader.ScaleMode_RawSize
                text: qsTr("Raw Size")
            }
            ScaleMenuItem {
                group: buttons
                pageReader: root.pageReader
                scaleMode: PageReader.ScaleMode_AutoFitAreaSize
                text: qsTr("Adjust Size")
            }
            ScaleMenuItem {
                group: buttons
                pageReader: root.pageReader
                scaleMode: PageReader.ScaleMode_AutoFitAreaWidth
                text: qsTr("Adjust Width")
            }
            ScaleMenuItem {
                group: buttons
                pageReader: root.pageReader
                scaleMode: PageReader.ScaleMode_AutoFitAreaHeight
                text: qsTr("Adjust Height")
            }
        }
        Text {
            text: qsTr("Fix Scale")
        }
        GridLayout {
            columns: 2
            ScaleMenuItem {
                group: buttons
                pageReader: root.pageReader
                scaleMode: PageReader.ScaleMode_FixWidthByRatio
                text: qsTr("Fix Width Ratio")
            }
            ScaleMenuItem {
                group: buttons
                pageReader: root.pageReader
                scaleMode: PageReader.ScaleMode_FixHeightByRatio
                text: qsTr("Fix Height Ratio")
            }
            ScaleMenuItem {
                group: buttons
                pageReader: root.pageReader
                scaleMode: PageReader.ScaleMode_FixWidthByPixel
                text: qsTr("Fix Width Pixel")
            }
            ScaleMenuItem {
                group: buttons
                pageReader: root.pageReader
                scaleMode: PageReader.ScaleMode_FixHeightByPixel
                text: qsTr("Fix Height Pixel")
            }
        }
    }
}
