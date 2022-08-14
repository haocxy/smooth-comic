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

        ToolBar {
            implicitWidth: parent.width
            implicitHeight: 64
            background: Rectangle {
                anchors.fill: parent
                color: "#c8c8c8"
            }

            RowLayout {
                anchors.fill: parent
                MyToolButton {
                    Layout.margins: 12
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
                    Layout.margins: 12
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                }
            }
        }

        FileListArea {
            fileChooser: idFileChooser
            implicitWidth: idWindow.width
            Layout.fillHeight: true
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
