import QtQuick
import QtQuick.Controls

Rectangle {
    id: idEntry
    required property string name
    implicitHeight: idFileName.contentHeight + 32

    Rectangle {
        width: parent.width - 4
        height: parent.height - 4
        anchors.centerIn: parent

        border {
            color: "#c8c8c8"
            width: 1
        }

        Text {
            id: idFileName
            anchors.centerIn: parent
            width: parent.width * 0.8
            height: contentHeight
            text: idEntry.name
            wrapMode: Text.Wrap
        }

    }
}
