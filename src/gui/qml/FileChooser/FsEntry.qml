import QtQuick
import QtQuick.Controls

Rectangle {
    id: idEntry
    required property string name
    required property string path
    required property bool isDir
    implicitHeight: idFileName.contentHeight + idFileName.padding + 12

    signal clicked(string path)

    color: isDir ? "#c6f3f2" : "white"
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
                idEntry.clicked(idEntry.path)
            }
        }
    }
}
