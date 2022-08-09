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
            Layout.preferredHeight: idCurrDir.contentHeight + 36
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
                    Layout.preferredHeight: 36
                    Layout.margins: 8

                    color: idCurrDir.focus ? idWindow.color : "#f5f5f5"
                    width: parent.width * 0.6

                    radius: 4

                    TextInput {
                        id: idCurrDir
                        anchors.fill: parent
                        leftPadding: 8
                        verticalAlignment: TextInput.AlignVCenter
                        text: idFileChooser.currDir
                        focus: false
                        onEditingFinished: {
                            Qt.inputMethod.hide()
                            idFileChooser.currDir = text
                            focus = false
                        }
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

            MouseArea {
                anchors.fill: parent
                onPressed: e => {
                    idCurrDir.focus = false
                    e.accepted = false
                }
            }
        }
    }
}
