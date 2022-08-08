import QtQuick
import QtQuick.Controls

Rectangle {
    id: idEntry
    width: idRoot.width
    height: idFileName.contentHeight + 32
    required property string name

    Text {
        id: idFileName
        text: parent.name
    }
}
