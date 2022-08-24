import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import myapp
import myapp.Global
import myapp.Util

Rectangle {
    id: idRoot
    required property string comicPath

    function switchState() {
        if (state == "controll") {
            state = "read"
            idTopBarAnimOnY.stop()
            idTopBarAnimOnY.from = idTopBar.y
            idTopBarAnimOnY.to = -idTopBar.height
            idTopBarAnimOnY.start()
        } else {
            window.switchToNormalState()
            state = "controll"
            idTopBarAnimOnY.stop()
            idTopBarAnimOnY.from = idTopBar.y
            idTopBarAnimOnY.to = 0
            idTopBarAnimOnY.start()
        }
    }

    function inWindowMoveArea(gpos: point): bool {
        if (idTopBar.contains(idTopBar.mapFromGlobal(gpos))) {
            return idTopBar.inWindowMoveArea(gpos)
        }
        if (idBottomBar.contains(idBottomBar.mapFromGlobal(gpos))) {
            return idBottomBar.inWindowMoveArea(gpos)
        }
        return false
    }

    state: "controll"

    states: [
        State {
            name: "controll"
            PropertyChanges {
                target: idRoot
                color: GlobalStyle.readerBgColor4ControllState
            }
            PropertyChanges {
                target: idBottomBar
                y: idRoot.height - idBottomBar.height
            }
        },
        State {
            name: "read"
            PropertyChanges {
                target: idRoot
                color: GlobalStyle.readerBgColor4ReadState
            }
            PropertyChanges {
                target: idBottomBar
                y: idRoot.height
            }
        }
    ]

    transitions: [
        Transition {
            reversible: true
            from: "controll"; to: "read"
            ParallelAnimation {
                ColorAnimation {
                    property: "color"
                }
                NumberAnimation {
                    property: "y"
//                    duration: 100
                }
                NumberAnimation {
                    property: "opacity"
                }
            }
        }
    ]

    ToolBar {
        id: idTopBar
        x: 0
        y: state === "read" ? -idTopBar.height : 0
        z: 100
        implicitWidth: parent.width
        implicitHeight: GlobalStyle.topBarHeight
        background: Rectangle {
            anchors.fill: parent
            color: GlobalStyle.titleBarColor
        }

        RowLayout {
            id: idTopBarLayout
            anchors.fill: parent
            MyToolButton {
                Layout.topMargin: 12
                Layout.bottomMargin: 12
                Layout.leftMargin: 8
                Layout.rightMargin: 8
                Layout.fillHeight: true
                Layout.preferredWidth: height
                iconHeight: GlobalStyle.topBarIconHeight
                text: "\ue5c4"
                onClicked: {
                    window.goBack()
                }
            }
        }

        // 为了让窗口标题栏的状态切换具有连贯性，
        // 需要确保窗口状态切换开始于顶部栏状态切换的结束时，
        // 而动画对象的 onFinished 信号无法在和其它机制例如transitions等配合时触发，
        // 所以对顶部状态栏的状态切换和动画效果单独手动处理，不使用状态机制
        NumberAnimation on y {
            id: idTopBarAnimOnY
            running: false
            onFinished: {
                // 这里只处理切换到read状态时的情况
                // 切换到controll状态时要先切换窗口标题栏的状态
                if (idRoot.state === "read") {
                    window.switchToReadState()
                }
            }
        }

        function inWindowMoveArea(gpos: point): bool {
            var lpos = idTopBarLayout.mapFromGlobal(gpos)
            return !idTopBarLayout.childAt(lpos.x, lpos.y)
        }
    }

    PageReader {
        id: idPageReader
        x: 0
        y: idRoot.state === "read" ? 0 : idTopBar.height
        width: idRoot.width
        height: idRoot.state === "read" ? idRoot.height : idRoot.height - idTopBar.height - idBottomBar.height
        controller: idController

        Component.onCompleted: {
            idController.filePath = Qt.binding(function() {return idRoot.comicPath})
        }
    }

    ToolBar {
        id: idBottomBar
        x: 0
        y: idRoot.height - height
        z: 100
        implicitWidth: parent.width
        implicitHeight: GlobalStyle.bottomBarHeight
        background: Rectangle {
            anchors.fill: parent
            color: GlobalStyle.titleBarColor
        }

        RowLayout {
            id: idBottomBarLayout
            anchors.fill: parent
            Item {
                id: idBottomLeftStrech
                Layout.fillWidth: true
                Layout.fillHeight: true
            }
            MyToolButton {
                Layout.topMargin: 12
                Layout.bottomMargin: 12
                Layout.leftMargin: 8
                Layout.rightMargin: 8
                Layout.fillHeight: true
                Layout.preferredWidth: height
                iconHeight: GlobalStyle.bottomBarIconHeight
                text: "\ue5c4"; onClicked: window.goBack()
            }
            MyToolButton {
                // scale
                Layout.topMargin: 12
                Layout.bottomMargin: 12
                Layout.leftMargin: 8
                Layout.rightMargin: 8
                Layout.fillHeight: true
                Layout.preferredWidth: height
                iconHeight: GlobalStyle.bottomBarIconHeight
                text: "\ue8b6"; onClicked: idScaleMenu.visible = !idScaleMenu.visible
            }
            Item {
                id: idBottomRightStrech
                Layout.fillWidth: true
                Layout.fillHeight: true
            }
        }

        function inWindowMoveArea(gpos: point): bool {
            if (idBottomLeftStrech.contains(idBottomLeftStrech.mapFromGlobal(gpos))) {
                return true
            }
            if (idBottomRightStrech.contains(idBottomRightStrech.mapFromGlobal(gpos))) {
                return true
            }
            return false
        }
    }

    Item {
        id: idClickControlls
        anchors.fill: parent
        opacity: 0

        function controll(gpos) {
            var lpos = mapFromGlobal(gpos)
            var item = childAt(lpos.x, lpos.y)
            if (!item) {
                switchState()
                return
            }
            if (item.handle) {
                item.handle()
            }
        }

        Rectangle {
            width: parent.width * (1 - 0.3 * 2)
            height: parent.height * ((1 - 0.6) / 2)
            opacity: 0.5
            color: "blue"
            anchors.top: parent.top
            anchors.horizontalCenter: parent.horizontalCenter

            function handle() {
                idController.switchUpPage()
            }
        }

        Rectangle {
            width: parent.width * (1 - 0.3 * 2)
            height: parent.height * ((1 - 0.6) / 2)
            opacity: 0.5
            color: "blue"
            anchors.bottom: parent.bottom
            anchors.horizontalCenter: parent.horizontalCenter

            function handle() {
                idController.switchDownPage()
            }
        }

        Rectangle {
            width: parent.width * 0.3
            height: parent.height * 0.6
            opacity: 0.5
            color: "blue"
            anchors.left: parent.left
            anchors.verticalCenter: parent.verticalCenter

            function handle() {
                idController.switchLeftPage()
            }
        }

        Rectangle {
            width: parent.width * 0.3
            height: parent.height * 0.6
            opacity: 0.5
            color: "blue"
            anchors.right: parent.right
            anchors.verticalCenter: parent.verticalCenter

            function handle() {
                idController.switchRightPage()
            }
        }

    }

    PinchArea {
        id: idPinchArea
        property real currScale: 1
        anchors.fill: parent

        onPinchUpdated: e => {
                            if (e.previousScale !== 0) {
                                idController.relativelyScale(e.scale / e.previousScale)
                            }
                        }

        MouseArea {
            id: idMouseArea
            anchors.fill: parent
            onClicked: e => {
                            if (!idMouseArea.moved) {
                                idClickControlls.controll(mapToGlobal(e.x, e.y))
                            }
                        }
            property real prevX: 0
            property real prevY: 0
            property bool moved: false
            onPressed: e => {
                           idMouseArea.prevX = e.x
                           idMouseArea.prevY = e.y
                           idMouseArea.moved = false
                       }

            onPositionChanged: e => {
                                   var dx = (e.x - prevX) * Screen.devicePixelRatio
                                   var dy = (e.y - prevY) * Screen.devicePixelRatio
                                   idController.translateBy(dx, dy)
                                   idMouseArea.prevX = e.x
                                   idMouseArea.prevY = e.y
                                   idMouseArea.moved = true
                               }
        }

    }

    ScaleMenu {
        anchors.centerIn: parent
        id: idScaleMenu
        pageReader: idPageReader
    }

    Controller {
        id: idController
        guiEngine: $engine
    }
}
