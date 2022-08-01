import QtQuick
import QtQuick.Controls
import QtQuick.Layouts


Rectangle {
    property int span: 100
    property bool isLandscape:
        Screen.orientation === Qt.LandscapeOrientation

    color: "#c8c8c8"

    GridLayout {
        width: parent.width
        height: parent.height
        flow: parent.isLandscape ? GridLayout.TopToBottom : GridLayout.LeftToRight

        ControlButton {
            buttonText: "\ue2c8"
        }

        ControlButton {
            buttonText: "\ue41a"
        }
    }
}
