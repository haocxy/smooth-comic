import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Qt.labs.platform

import myapp

Rectangle {

    id: idWindow

    FileTree {
        id: idFileTree
    }

    ColumnLayout {
        width: parent.width
        height: parent.height

        Rectangle {
            Layout.fillWidth: true
            Layout.preferredHeight: guiTextCurrDir.contentHeight + 32
            Layout.alignment: Qt.AlignVCenter
            id: idDirArea
            width: parent.width
            color: "#c8c8c8"
            z: 1

            Row {

                anchors.verticalCenter: parent.verticalCenter
                padding: parent.height / 2
                spacing: parent.height / 2

                Text {
                    id: guiTextCurrDir
                    text: idFileTree.currDir
                    wrapMode: Text.WordWrap
                }
            }
        }

        GridView {
            id: idGridLayout
            property int goodWidth: 100
            Layout.fillHeight: true
            implicitWidth: idWindow.width
            cellWidth: Math.min(idWindow.width, idWindow.width / Math.floor(idWindow.width / goodWidth))

            model: idFileTree.entries
            delegate: FileTreeEntry {
                implicitWidth: idGridLayout.cellWidth
            }
        }
    }
}
