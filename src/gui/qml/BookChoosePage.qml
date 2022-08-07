import QtQuick
import QtQuick.Controls
import Qt.labs.platform

import myapp

Rectangle {

    FileTree {

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

