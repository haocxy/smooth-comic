import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Qt.labs.platform

import myapp

Rectangle {

    id: idWindow

    FileChooser {
        id: idFileChooser
    }

    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        TopArea {
            id: idTopArea
            Layout.fillWidth: true
            Layout.preferredHeight: goodHeight
            Layout.alignment: Qt.AlignVCenter
        }

        FileListArea {
            fileChooser: idFileChooser
            implicitWidth: idWindow.width
            Layout.fillHeight: true
        }
    }
}
