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

        Flickable {
            implicitWidth: idWindow.width
            Layout.fillHeight: true
            contentWidth: idGridLayout.width
            contentHeight: idGridLayout.height
            GridLayout {
                id: idGridLayout
                property int goodWidth: 100
                implicitWidth: idWindow.width
                columns: Math.max(1, Math.floor(idWindow.width / idGridLayout.goodWidth))
                rowSpacing: 0
                columnSpacing: 0

                Repeater {
                    model: idFileChooser.entries
                    delegate: FileTreeEntry {
                        Layout.alignment: Qt.AlignTop
                        implicitWidth: Math.min(idWindow.width, idWindow.width / Math.floor(idWindow.width / idGridLayout.goodWidth))
                    }
                }
            }
        }
    }
}
