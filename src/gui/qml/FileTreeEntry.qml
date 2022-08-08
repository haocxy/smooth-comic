import QtQuick
import QtQuick.Controls

Rectangle {
    id: idEntry
    required property string name
    implicitHeight: idFileName.contentHeight + 32

    border {
        color: "#c8c8c8"
        width: 1
    }

    Text {
        id: idFileName
        anchors.centerIn: parent
        width: parent.width * 0.8
        height: contentHeight
        text: parent.name
        wrapMode: Text.WrapAnywhere
    }
}
