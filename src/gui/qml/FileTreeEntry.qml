import QtQuick
import QtQuick.Controls

Rectangle {
    id: idEntry
    height: idFileName.contentHeight + 32
    required property string name

    Text {
        id: idFileName
        text: parent.name
        wrapMode: Text.Wrap
    }
}
