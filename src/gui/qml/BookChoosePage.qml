import QtQuick
import QtQuick.Controls
import Qt.labs.platform

import myapp

Rectangle {

    FileTree {
        id: fileTree
    }

    Rectangle {
        width: parent.width
        height: guiTextCurrDir.contentHeight + 16
        color: "#c8c8c8"

        Row {

            anchors.verticalCenter: parent.verticalCenter
            padding: parent.height / 2
            spacing: parent.height / 2

            Text {
                id: guiTextCurrDir
                text: fileTree.currDir
                wrapMode: Text.WordWrap
            }

        }
    }

    Button {
        anchors.centerIn: parent
        text: "Open File"
        onClicked: fileDialog.open()
    }

    FileDialog {
        id: fileDialog
    }
}

