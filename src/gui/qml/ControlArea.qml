import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Qt.labs.platform

Rectangle {
    property int span: 80
    property bool isLandscape: false

    color: "#c8c8c8"

    GridLayout {
        width: parent.width
        height: parent.height
        flow: parent.isLandscape ? GridLayout.TopToBottom : GridLayout.LeftToRight

        ControlButton {
            buttonText: "\ue2c8"
            onClicked: fileDialog.open()
        }

        ControlButton {
            buttonText: "\ue41a"
        }
    }

    FileDialog {
        id: fileDialog
    }
}
