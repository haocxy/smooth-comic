import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import "./FileChooser"
import "./BookReader"
import "./WindowStateSwitcher"
import "./Global"

Item{
    // 这个QML文件作为第一个加载的QML文件，可以被所有模块访问，
    // 所以id必须是可以区分全局对象的名字，不能是root等普通名字
    id: window
    objectName: "window"
    visible: true
    width: 800
    height: 600
    property bool isMin: false
    property bool isMax: false
    property bool isFullScreen: false

    function inWindowMoveArea(x: int, y: int) : bool {
        var gpos = Qt.point(x, y)
        if (titleBar.contains(titleBar.mapFromGlobal(gpos))) {
            return titleBar.inWindowMoveArea(gpos)
        }
        if (scenes.currentItem && scenes.currentItem.inWindowMoveArea) {
            return scenes.currentItem.inWindowMoveArea(gpos)
        }
        return false
    }

    function openFile(path) {
        scenes.push(compReaderScene, {comicPath: path})
    }

    function goBack() {
        function tryLocalGoBack() {
            if (!scenes.currentItem) {
                return false
            }
            if (!scenes.currentItem.localGoBack) {
                return false
            }
            return scenes.currentItem.localGoBack()
        }
        // 尝试局部返回，若执行了局部返回则不继续处理
        if (tryLocalGoBack()) {
            return
        }

        // 若没有执行局部返回则全局返回
        if (scenes.depth > 1) {
            scenes.pop()
        } else {
            Qt.quit()
        }
    }

    function switchToReadState() {
        titleBar.state = "read"
    }

    function switchToNormalState() {
        titleBar.state = "normal"
    }

    Connections {
        target: $engine
        function onKeyBackReleased() {
            window.goBack()
        }
    }

    Component.onCompleted: {
        scenes.push(compFileChooseScene)
    }

    ColumnLayout {
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: inputMethodStrech.top
        height: parent.height - inputMethodStrech.height
        spacing: 0

        Rectangle {
            id: titleBar
            objectName: "windowTitleBar"
            visible: $engine.isWindowed
            z: 100
            implicitWidth: parent.width
            implicitHeight: Style.titleBarHeight
            state: "normal"
            RowLayout {
                anchors.fill: parent
                WindowStateSwitcher {
                    id: windowStateSwitcher
                    objectName: "windowStateSwitcher"
                    Layout.alignment: Qt.AlignRight
                    btnBgColor: titleBar.color
                }
            }
            states: [
                State {
                    name: "normal"
                    PropertyChanges {
                        target: titleBar
                        color: Style.titleBarColor
                    }
                },
                State {
                    name: "read"
                    PropertyChanges {
                        target: titleBar
                        color: Style.titleBarColor4ReadState
                    }
                }
            ]

            function inWindowMoveArea(gpos: point): bool {
                return !windowStateSwitcher.contains(windowStateSwitcher.mapFromGlobal(gpos))
            }
        }

        StackView {
            Layout.fillWidth: true
            Layout.fillHeight: true
            id: scenes

            Component.onCompleted: {
                if ($engine.isWindowed) {
                    popEnter = null
                    popExit = null
                    pushEnter = null
                    pushExit = null
                    replaceEnter = null
                    replaceExit = null
                }
            }
        }
    }

    Rectangle {
        id: inputMethodStrech
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        property int heightDelta: Qt.inputMethod.visible ? (Qt.platform.os === "android" ? 36 : 0) : 0
        height: Math.ceil(Qt.inputMethod.keyboardRectangle.height / Screen.devicePixelRatio) + heightDelta
    }

    Component {
        id: compFileChooseScene
        FileChooserScene {
            width: window.width
            height: scenes.height
        }
    }

    Component {
        id: compReaderScene
        BookReaderScene {
            width: window.width
            height: scenes.height
        }
    }

    function openInputMethod(inputItem) {
        Qt.inputMethod.show()
        inputItem.forceActiveFocus()
    }

}
