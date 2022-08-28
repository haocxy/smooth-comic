import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import myapp
import "../Global"
import "../Util"

Rectangle {
    id: root
    required property string comicPath

    function switchState() {
        if (state == "controll") {
            state = "read"
            topBarAnimOnY.stop()
            topBarAnimOnY.from = topBar.y
            topBarAnimOnY.to = -topBar.height
            topBarAnimOnY.start()
        } else {
            window.switchToNormalState()
            state = "controll"
            topBarAnimOnY.stop()
            topBarAnimOnY.from = topBar.y
            topBarAnimOnY.to = 0
            topBarAnimOnY.start()
        }
    }

    function inWindowMoveArea(gpos: point): bool {
        if (topBar.contains(topBar.mapFromGlobal(gpos))) {
            return topBar.inWindowMoveArea(gpos)
        }
        if (bottomBar.contains(bottomBar.mapFromGlobal(gpos))) {
            return bottomBar.inWindowMoveArea(gpos)
        }
        return false
    }

    state: "controll"

    states: [
        State {
            name: "controll"
            PropertyChanges {
                target: root
                color: Style.readerBgColor4ControllState
            }
            PropertyChanges {
                target: bottomBar
                y: root.height - bottomBar.height
            }
        },
        State {
            name: "read"
            PropertyChanges {
                target: root
                color: Style.readerBgColor4ReadState
            }
            PropertyChanges {
                target: bottomBar
                y: root.height
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
        id: topBar
        x: 0
        y: state === "read" ? -topBar.height : 0
        z: 100
        implicitWidth: parent.width
        implicitHeight: Style.topBarHeight
        background: Rectangle {
            anchors.fill: parent
            color: Style.titleBarColor
            opacity: Style.readControlAreaBgOpacity
        }

        RowLayout {
            id: topBarLayout
            anchors.fill: parent
            ControlButton {
                Layout.topMargin: 12
                Layout.bottomMargin: 12
                Layout.leftMargin: 8
                Layout.rightMargin: 8
                Layout.fillHeight: true
                Layout.preferredWidth: height
                iconHeight: Style.topBarIconHeight
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
            id: topBarAnimOnY
            running: false
            onFinished: {
                // 这里只处理切换到read状态时的情况
                // 切换到controll状态时要先切换窗口标题栏的状态
                if (root.state === "read") {
                    window.switchToReadState()
                }
            }
        }

        function inWindowMoveArea(gpos: point): bool {
            var lpos = topBarLayout.mapFromGlobal(gpos)
            return !topBarLayout.childAt(lpos.x, lpos.y)
        }
    }


    ScaleMenu {
        id: scaleMenu
        initX: (root.width - width) / 2
        initY: (root.height - height) / 2
        dragMinX: Style.popMenuMargin
        dragMaxX: root.width - width - Style.popMenuMargin
        dragMinY: Style.popMenuMargin + topBar.y + topBar.height
        dragMaxY: bottomBar.y - height - Style.popMenuMargin
        pageReader: pageReader
    }

    PageReader {
        id: pageReader
        anchors.fill: parent
        controller: controller
        Component.onCompleted: {
            controller.filePath = Qt.binding(function() {return root.comicPath})
        }
    }

    ToolBar {
        id: bottomBar
        x: 0
        y: root.height - height
        z: 100
        implicitWidth: parent.width
        implicitHeight: Style.bottomBarHeight
        background: Rectangle {
            anchors.fill: parent
            color: Style.titleBarColor
            opacity: Style.readControlAreaBgOpacity
        }

        RowLayout {
            id: bottomBarLayout
            anchors.fill: parent
            Item {
                id: bottomLeftStrech
                Layout.fillWidth: true
                Layout.fillHeight: true
            }
            ControlButton {
                Layout.topMargin: 12
                Layout.bottomMargin: 12
                Layout.leftMargin: 8
                Layout.rightMargin: 8
                Layout.fillHeight: true
                Layout.preferredWidth: height
                iconHeight: Style.bottomBarIconHeight
                text: "\ue5c4"; onClicked: window.goBack()
            }
            ControlButton {
                // scale
                Layout.topMargin: 12
                Layout.bottomMargin: 12
                Layout.leftMargin: 8
                Layout.rightMargin: 8
                Layout.fillHeight: true
                Layout.preferredWidth: height
                iconHeight: Style.bottomBarIconHeight
                text: "\ue8b6"; onClicked: scaleMenu.visible = !scaleMenu.visible
            }
            Item {
                id: bottomRightStrech
                Layout.fillWidth: true
                Layout.fillHeight: true
            }
        }

        function inWindowMoveArea(gpos: point): bool {
            if (bottomLeftStrech.contains(bottomLeftStrech.mapFromGlobal(gpos))) {
                return true
            }
            if (bottomRightStrech.contains(bottomRightStrech.mapFromGlobal(gpos))) {
                return true
            }
            return false
        }
    }

    Item {
        id: clickControlls
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
                controller.switchUpPage()
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
                controller.switchDownPage()
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
                controller.switchLeftPage()
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
                controller.switchRightPage()
            }
        }

    }

    PinchArea {
        property real currScale: 1
        anchors.fill: parent

        onPinchUpdated: e => {
                            if (e.previousScale !== 0) {
                                controller.relativelyScale(e.scale / e.previousScale)
                            }
                        }

        MouseArea {
            anchors.fill: parent
            onClicked: e => {
                            if (!moved) {
                                clickControlls.controll(mapToGlobal(e.x, e.y))
                            }
                        }
            property real prevX: 0
            property real prevY: 0
            property bool moved: false
            onPressed: e => {
                           prevX = e.x
                           prevY = e.y
                           moved = false
                       }

            onPositionChanged: e => {
                                   var dx = (e.x - prevX) * Screen.devicePixelRatio
                                   var dy = (e.y - prevY) * Screen.devicePixelRatio
                                   controller.translateBy(dx, dy)
                                   prevX = e.x
                                   prevY = e.y
                                   moved = true
                               }
        }

    }

    Controller {
        id: controller
        guiEngine: $engine
    }
}
