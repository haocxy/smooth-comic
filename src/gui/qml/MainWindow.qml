import QtQuick
import QtQuick.Controls

import myapp.FileChooser


Item{
    id: window
    visible: true
    width: 800
    height: 600

    function globalBack() {
        if (idSceneStack.depth > 1) {
            idSceneStack.pop()
        } else {
            Qt.quit()
        }
    }

    Component.onCompleted: {
        idSceneStack.push(idCompBookChooseScene)
    }

    StackView {
        id: idSceneStack
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: idStrech.top
        height: parent.height - idStrech.height
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
        FileChooserArea {
            width: window.width
            height: idSceneStack.height
        }
    }

    Component {
        id: idCompBookReadScene
        BookReadPage {
            width: window.width
            height: idSceneStack.height
        }
    }

    function openInputMethod(inputItem) {
        Qt.inputMethod.show()
        inputItem.forceActiveFocus()
    }

}
