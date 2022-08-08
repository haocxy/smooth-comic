import QtQuick
import QtQuick.Controls

Rectangle {
    id: idEntry
    required property string name
    implicitHeight: idFileName.contentHeight + 32


    Text {
        id: idFileName
        anchors.centerIn: parent
        width: parent.width * 0.8
        height: contentHeight
        text: parent.name
        wrapMode: Text.WrapAnywhere
    }
}
