import QtQuick
import QtQuick.Controls
import QtQuick.Layouts


Rectangle {
    color: "#c8c8c8"

    GridLayout {
        width: parent.width
        height: parent.height

        ControlButton {
            buttonText: "\ue2c8"
        }

        ControlButton {
            buttonText: "\ue41a"
        }
    }
}
