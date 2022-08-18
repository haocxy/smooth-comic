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

    function openDir(path) {
        idFileChooser.openDir(path, 0)
    }

    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        ToolBar {
            z: 100
            implicitWidth: parent.width
            implicitHeight: 64
            background: Rectangle {
                anchors.fill: parent
                color: "#c8c8c8"
            }

            RowLayout {
                anchors.fill: parent
                MyToolButton {
                    Layout.topMargin: 12
                    Layout.bottomMargin: 12
                    Layout.leftMargin: 8
                    Layout.rightMargin: 8
                    Layout.fillHeight: true
                    Layout.preferredWidth: height
                    id: idBackBtn
                    text: "\ue5c4"
                    onClicked: {
                        idFileChooser.goBack()
                    }
                }
                AddrBarArea {
                    id: idAddrBarArea
                    Layout.topMargin: 12
                    Layout.bottomMargin: 12
                    Layout.leftMargin: 0
                    Layout.rightMargin: 8
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    dir: idFileChooser.currDir
                    onShouldOpenDir: p => openDir(p)
                }
            }
        }

        FileListArea {
            fileChooser: idFileChooser
            implicitWidth: idWindow.width
            Layout.fillHeight: true
            onShouldOpenDir: p => idWindow.openDir(p)
        }
    }

    MouseArea {
        anchors.fill: parent
        onPressed: e => {
                       idAddrBarArea.removeFocus()
                       e.accepted = false
                   }
    }
}
