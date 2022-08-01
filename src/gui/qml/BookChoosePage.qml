import QtQuick
import QtQuick.Controls
import Qt.labs.platform

Rectangle {
    Button {
        anchors.centerIn: parent
        text: "Open File"
        onClicked: fileDialog.open()
    }

    FileDialog {
        id: fileDialog
    }
}

