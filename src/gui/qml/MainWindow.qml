import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import myapp.FileChooser
import myapp.BookReader
import myapp.WindowStateSwitcher
import myapp.Global

Item{
    id: window
    visible: true
    width: 800
    height: 600
    property bool isMin: false
    property bool isMax: false
    property bool isFullScreen: false

    function openFile(path) {
        idSceneStack.push(idCompBookReadScene, {comicPath: path})
    }

    function goBack() {
        function tryLocalGoBack() {
            if (!idSceneStack.currentItem) {
                return false
            }
            if (!idSceneStack.currentItem.localGoBack) {
                return false
            }
            return idSceneStack.currentItem.localGoBack()
        }
        // 尝试局部返回，若执行了局部返回则不继续处理
        if (tryLocalGoBack()) {
            return
        }

        // 若没有执行局部返回则全局返回
        if (idSceneStack.depth > 1) {
            idSceneStack.pop()
        } else {
            Qt.quit()
        }
    }

    Connections {
        target: $engine
        function onKeyBackReleased() {
            window.goBack()
        }
    }

    Component.onCompleted: {
        idSceneStack.push(idCompBookChooseScene)
    }

    ColumnLayout {
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: idStrech.top
        height: parent.height - idStrech.height
        spacing: 0

        Rectangle {
            objectName: "windowTitleBar"
            visible: $engine.isWindowed
            z: 100
            implicitWidth: parent.width
            implicitHeight: GlobalStyle.titleBarHeight
            color: GlobalStyle.titleBarColor
            RowLayout {
                anchors.fill: parent
                WindowStateSwitcher {
                    objectName: "windowStateSwitcher"
                    Layout.alignment: Qt.AlignRight
                }
            }
        }

        StackView {
            Layout.fillWidth: true
            Layout.fillHeight: true
            id: idSceneStack
        }
    }

    Rectangle {
        id: idStrech
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        property int heightDelta: Qt.inputMethod.visible ? (Qt.platform.os === "android" ? 36 : 0) : 0
        height: Math.ceil(Qt.inputMethod.keyboardRectangle.height / Screen.devicePixelRatio) + heightDelta
    }

    Component {
        id: idCompBookChooseScene
        FileChooserScene {
            width: window.width
            height: idSceneStack.height
        }
    }

    Component {
        id: idCompBookReadScene
        BookReaderScene {
            width: window.width
            height: idSceneStack.height
        }
    }

    function openInputMethod(inputItem) {
        Qt.inputMethod.show()
        inputItem.forceActiveFocus()
    }

}
