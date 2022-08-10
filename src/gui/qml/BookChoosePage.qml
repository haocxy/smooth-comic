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
        }

        BookChoosePageFileListArea {
            fileChooser: idFileChooser

            MouseArea {
                anchors.fill: parent
                onPressed: e => {
                    idTopArea.removeFocus()
                    e.accepted = false
                }
            }
        }
    }
}
