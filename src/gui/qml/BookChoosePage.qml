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

        BookChoosePageTopArea {
            id: idTopArea
            Layout.fillWidth: true
            Layout.preferredHeight: goodHeight
            Layout.alignment: Qt.AlignVCenter
        }

        BookChoosePageFileListArea {
            fileChooser: idFileChooser
            implicitWidth: idWindow.width
            Layout.fillHeight: true
        }
    }

    MouseArea {
        anchors.fill: parent
        onPressed: e => {
            var gpos = mapToGlobal(e.x, e.y)
            if (!idTopArea.contains(idTopArea.mapFromGlobal(gpos.x, gpos.y))) {
                idTopArea.removeFocus()
            }
            e.accepted = false
        }
    }
}
