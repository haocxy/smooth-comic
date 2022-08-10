import QtQuick
import QtQuick.Controls

Rectangle {
    id: idEntry
    required property string name
    required property string path
    required property bool isDir
    required property var fileChooser
    implicitHeight: idFileName.contentHeight + 20

    Rectangle {
        width: parent.width - 4
        height: parent.height - 4
        anchors.centerIn: parent
        color: isDir ? "#c6f3f2" : parent.color
        radius: 4

        border {
            color: "#c8c8c8"
            width: 1
        }

        Text {
            id: idFileName
            anchors.fill: parent
            padding: 8
            text: idEntry.name
            wrapMode: Text.Wrap
        }

        MouseArea {
            anchors.fill: parent
            onClicked: fileChooser.currDir = path
        }
    }
}
