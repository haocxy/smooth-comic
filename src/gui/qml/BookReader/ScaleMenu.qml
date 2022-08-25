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
    height: segCount * Style.popMenuTitleAreaHeight + (autoScaleAreaLayout.children.length + fixedScaleAreaLayout.children.length) * Style.popMenuContentRowHeight + (segCount - 1) * Style.popMenuSegGap
    modal: true
    padding: Style.popMenuContentMargin
    background: Rectangle {
        color: Style.popMenuBgColor
        radius: Style.popMenuRadius
        opacity: Style.popMenuOpacity
    }

    ColumnLayout {
        anchors.fill: parent
        id: rootLayout
        Text {
            Layout.fillWidth: true; Layout.fillHeight: true
            antialiasing: true
            font { pixelSize: Style.popMenuTitleTextPixSize }
            text: qsTr("Auto Scale")
            verticalAlignment: Text.AlignVCenter
        }
        GridLayout {
            id: autoScaleAreaLayout
            Layout.fillWidth: true; Layout.fillHeight: true
            columns: 2
            ScaleMenuItem {
                scaleMode: PageReader.ScaleMode_RawSize
                text: qsTr("Raw Size")
            }
            ScaleMenuItem {
                scaleMode: PageReader.ScaleMode_AutoFitAreaSize
                text: qsTr("Adjust Size")
            }
            ScaleMenuItem {
                scaleMode: PageReader.ScaleMode_AutoFitAreaWidth
                text: qsTr("Adjust Width")
            }
            ScaleMenuItem {
                scaleMode: PageReader.ScaleMode_AutoFitAreaHeight
                text: qsTr("Adjust Height")
            }
        }
        Item {
            Layout.fillWidth: true
            Layout.preferredHeight: Style.popMenuSegGap
        }
        Text {
            Layout.fillWidth: true; Layout.fillHeight: true
            font { pixelSize: Style.popMenuTitleTextPixSize }
            text: qsTr("Fix Scale")
            verticalAlignment: Text.AlignVCenter
        }
        GridLayout {
            id: fixedScaleAreaLayout
            Layout.fillWidth: true; Layout.fillHeight: true
            columns: 2
            ScaleMenuItem {
                scaleMode: PageReader.ScaleMode_FixWidthByRatio
                text: qsTr("Fix Width Ratio")
            }
            ScaleMenuItem {
                scaleMode: PageReader.ScaleMode_FixHeightByRatio
                text: qsTr("Fix Height Ratio")
            }
            ScaleMenuItem {
                scaleMode: PageReader.ScaleMode_FixWidthByPixel
                text: qsTr("Fix Width Pixel")
            }
            ScaleMenuItem {
                scaleMode: PageReader.ScaleMode_FixHeightByPixel
                text: qsTr("Fix Height Pixel")
            }
        }
    }

    ButtonGroup { id: buttons }
}
