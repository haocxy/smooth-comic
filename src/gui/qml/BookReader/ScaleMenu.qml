import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import myapp
import "../Global"

Popup {
    required property var pageReader
    property int segCount: 2

    id: root
    z: ZOrder.popMenu
    width: 260
    height: segCount * GlobalStyle.popMenuTitleAreaHeight + (autoScaleAreaLayout.children.length + fixedScaleAreaLayout.children.length) * GlobalStyle.popMenuContentRowHeight + (segCount - 1) * GlobalStyle.popMenuSegGap
    modal: true
    padding: GlobalStyle.popMenuContentMargin
    background: Rectangle {
        color: GlobalStyle.popMenuBgColor
        radius: GlobalStyle.popMenuRadius
        opacity: GlobalStyle.popMenuOpacity
    }

    ButtonGroup { id: buttons }

    ColumnLayout {
        anchors.fill: parent
        id: rootLayout
        Text {
            Layout.fillWidth: true; Layout.fillHeight: true
            antialiasing: true
            font { pixelSize: GlobalStyle.popMenuTitleTextPixSize }
            text: qsTr("Auto Scale")
            verticalAlignment: Text.AlignVCenter
        }
        GridLayout {
            id: autoScaleAreaLayout
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
        Item {
            Layout.fillWidth: true
            Layout.preferredHeight: GlobalStyle.popMenuSegGap
        }
        Text {
            Layout.fillWidth: true; Layout.fillHeight: true
            font { pixelSize: GlobalStyle.popMenuTitleTextPixSize }
            text: qsTr("Fix Scale")
            verticalAlignment: Text.AlignVCenter
        }
        GridLayout {
            id: fixedScaleAreaLayout
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
