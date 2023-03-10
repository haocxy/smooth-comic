import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import myapp
import "../Global"

Rectangle {
    required property real initX
    required property real initY
    required property real dragMinX
    required property real dragMaxX
    required property real dragMinY
    required property real dragMaxY
    required property var pageReader
    property real hPinDist: 16
    property real vPinDist: 16

    id: root
    z: ZOrder.popMenu
    width: 256
    height: titleBar.height + rootLayout.children.length * Style.popMenuContentRowHeight + Style.popMenuContentMargin * 2
    color: Style.popMenuBgColor
    radius: Style.popMenuRadius
    opacity: Style.popMenuOpacity
    visible: false

    Component.onCompleted: {
        x = initX
        y = initY
    }

    property int eNotPined: 0
    property int ePinTop: 1
    property int ePinBottom: 2
    property int ePinLeft: 3
    property int ePinRight: 4
    property int hPinType: eNotPined
    property int vPinType: eNotPined
    property real hPinOffset: 0
    property real vPinOffset: 0

    onHPinTypeChanged: console.log("qmllog: hPinType: ", hPinType)
    onVPinTypeChanged: console.log("qmllog: vPinType: ", vPinType)
    onXChanged: {
        if (root.x - dragMinX < hPinDist) {
            hPinType = ePinLeft
            hPinOffset = root.x - dragMinX
        } else if (dragMaxX - root.x < hPinDist) {
            hPinType = ePinRight
            hPinOffset = root.x - dragMaxX
        } else {
            hPinType = eNotPined
            hPinOffset = 0
        }
    }
    onYChanged: {
        if (root.y - dragMinY < vPinDist) {
            vPinType = ePinTop
            vPinOffset = root.y - dragMinY
        } else if (dragMaxY - root.y < vPinDist) {
            vPinType = ePinBottom
            vPinOffset = root.y - dragMaxY
        } else {
            vPinType = eNotPined
            vPinOffset = 0
        }
    }
    onDragMinXChanged: {
        if (hPinType === ePinLeft) {
            root.x = dragMinX + hPinOffset
        }
    }
    onDragMaxXChanged: {
        if (hPinType === ePinRight) {
            root.x = dragMaxX + hPinOffset
        }
    }
    onDragMinYChanged: {
        if (vPinType === ePinTop) {
            root.y = dragMinY + vPinOffset
        }
    }
    onDragMaxYChanged: {
        if (vPinType === ePinBottom) {
            root.y = dragMaxY + vPinOffset
        }
    }

    MouseArea {
        id: mouseAreaForDrag
        anchors.fill: parent
        drag.target: parent
        drag.axis: Drag.XAndYAxis
        drag.minimumX: dragMinX
        drag.maximumX: dragMaxX
        drag.minimumY: dragMinY
        drag.maximumY: dragMaxY
    }



    Rectangle {
        id: titleBar
        x: 0; y: 0; width: parent.width; height: Style.popMenuTitleBarHeight
        color: Style.popMenuTitleBarBgColor
        radius: { width: 1; color: Style.popMenuRadius }
        Rectangle {
           x: 0; y: parent.height - parent.radius
           width: parent.width; height: parent.radius
           color: parent.color
        }
        Text {
            width: height
            height: parent.height
            anchors.top: parent.top
            anchors.right: parent.right
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            color: Style.popMenuTitleBarIconColor
            text: FontIcon.popMenuClose
            font.family: FontIcon.fontFamily
            font.pixelSize: Style.popMenuTitleButtonHeightRatio * height

            MouseArea {
                anchors.fill: parent
                onClicked: root.visible = false
            }
        }
    }

    GridLayout {
        id: rootLayout
        x: Style.popMenuContentMargin
        y: titleBar.height + Style.popMenuContentMargin
        width: root.width - Style.popMenuContentMargin * 2
        height: root.height - Style.popMenuContentMargin * 2 - titleBar.height
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


    ButtonGroup { id: buttons }
}
