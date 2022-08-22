import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Qt.labs.platform

import myapp
import myapp.Global
import myapp.Util
import myapp.WindowStateSwitcher

Rectangle {

    id: idWindow

    color: "#eeeeee"

    FileChooser {
        id: idFileChooser
    }

    function localGoBack() {
        if (idFileChooser.canGoBack) {
            idFileChooser.goBack()
            return true
        } else {
            return false
        }
    }

    function openDir(path) {
        idFileChooser.openDir(path, idEntryListArea.contentY)
        idEntryListArea.contentY = 0
    }

    function openFile(path) {
        window.openFile(path)
    }

    property bool isRestoringContentY: false
    property real restoredContentY: 0

    Connections {
        target: idFileChooser
        function onSigStartRestoreContentY(contentY) {
            isRestoringContentY = true
            restoredContentY = contentY
        }
    }

    Connections {
        target: idEntryListArea
        function onContentHeightChanged() {
            if (isRestoringContentY) {
                if (idEntryListArea.entryUiItemCount === idFileChooser.entryCount()) {
                    idEntryListArea.contentY = restoredContentY
                    isRestoringContentY = false
                }
            }
        }
    }

    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        ToolBar {
            z: 100
            implicitWidth: parent.width
            implicitHeight: GlobalStyle.titleBarHeight
            background: Rectangle {
                anchors.fill: parent
                color: GlobalStyle.titleBarColor
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
                WindowStateSwitcher {

                }
            }
        }

        FileListArea {
            id: idEntryListArea
            fileChooser: idFileChooser
            implicitWidth: idWindow.width
            Layout.fillHeight: true
            onShouldOpenDir: p => idWindow.openDir(p)
            onShouldOpenFile: p => idWindow.openFile(p)
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
