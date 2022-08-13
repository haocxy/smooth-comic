import QtQuick
import QtQuick.Controls

Rectangle {
    id: idEntry
    required property string name
    required property string path
    required property bool isDir
    required property var fileChooser
    required property Flickable fileListFlickable
    implicitHeight: idFileName.contentHeight + idFileName.padding + 14

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
            padding: 10
            text: idEntry.name
            wrapMode: Text.Wrap
        }

        MouseArea {
            anchors.fill: parent
            onClicked: {
                if (idEntry.isDir) {
                    fileChooser.openDir(idEntry.path, fileListFlickable.contentY)
                }
            }
        }
    }
}
