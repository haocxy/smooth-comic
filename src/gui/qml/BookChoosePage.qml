import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Qt.labs.platform

import myapp

Rectangle {

    id: idRoot

    FileTree {
        id: idFileTree
    }

    ColumnLayout {
        width: parent.width
        height: parent.width

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

        ListView {
            Layout.fillHeight: true
            width: idRoot.width
            model: idFileTree.entries

            delegate: Rectangle {
                id: idEntry
                width: idRoot.width
                height: idFileName.contentHeight + 32
                required property string name

                Text {
                    id: idFileName
                    text: parent.name
                }
            }
        }
    }
}
