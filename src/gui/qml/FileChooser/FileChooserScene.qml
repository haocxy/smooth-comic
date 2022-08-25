import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Qt.labs.platform

import myapp
import "../Global"
import "../Util"

Rectangle {

    id: idWindow

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

    function inWindowMoveArea(gpos: point): bool {
        if (idTopArea.contains(idTopArea.mapFromGlobal(gpos))) {
            return idTopArea.inWindowMoveArea(gpos)
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
            id: idTopArea
            z: 100
            implicitWidth: parent.width
            implicitHeight: Style.topBarHeight
            background: Rectangle {
                anchors.fill: parent
                color: Style.titleBarColor
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
                    iconHeight: Style.topBarIconHeight
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

            function inWindowMoveArea(gpos: point): bool {
                var lpos = mapFromGlobal(gpos)
                // 顶部栏的上面一部分和窗口连在一起，所以认为顶部栏的上面也可以用于移动窗口
                // 如果不把这部分算做可用于移动窗口，体验不好
                return lpos.y < idAddrBarArea.y
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
