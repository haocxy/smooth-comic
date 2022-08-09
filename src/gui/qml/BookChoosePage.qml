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

        Rectangle {
            Layout.fillWidth: true
            Layout.preferredHeight: guiTextCurrDir.contentHeight + 36
            Layout.alignment: Qt.AlignVCenter
            id: idDirArea
            width: parent.width
            color: "#c8c8c8"
            z: 1

            RowLayout {
                anchors.fill: parent
                spacing: 8

                Rectangle {
                    Layout.fillWidth: true
                    Layout.preferredHeight: 32
                    Layout.margins: 8

                    color: idWindow.color
                    width: parent.width * 0.6

                    radius: 4

                    TextInput {
                        id: guiTextCurrDir
                        anchors.fill: parent
                        leftPadding: 8
                        verticalAlignment: TextInput.AlignVCenter
                        text: idFileChooser.currDir
                    }
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
                        fileChooser: idFileChooser
                        Layout.alignment: Qt.AlignTop
                        implicitWidth: Math.min(idWindow.width, idWindow.width / Math.floor(idWindow.width / idGridLayout.goodWidth))
                    }
                }
            }
        }
    }
}
