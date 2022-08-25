import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Qt.labs.platform

import myapp
import "../Global"
import "../Util"

Rectangle {

    id: root

    FileChooser {
        id: fileChooser
    }

    function localGoBack() {
        if (fileChooser.canGoBack) {
            fileChooser.goBack()
            return true
        } else {
            return false
        }
    }

    function inWindowMoveArea(gpos: point): bool {
        if (topBar.contains(topBar.mapFromGlobal(gpos))) {
            return topBar.inWindowMoveArea(gpos)
        }
    }

    function openDir(path) {
        fileChooser.openDir(path, entriesArea.contentY)
        entriesArea.contentY = 0
    }

    function openFile(path) {
        window.openFile(path)
    }

    property bool isRestoringContentY: false
    property real restoredContentY: 0

    Connections {
        target: fileChooser
        function onSigStartRestoreContentY(contentY) {
            isRestoringContentY = true
            restoredContentY = contentY
        }
    }

    Connections {
        target: entriesArea
        function onContentHeightChanged() {
            if (isRestoringContentY) {
                if (entriesArea.entryUiItemCount === fileChooser.entryCount()) {
                    entriesArea.contentY = restoredContentY
                    isRestoringContentY = false
                }
            }
        }
    }

    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        ToolBar {
            id: topBar
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
                    iconHeight: Style.topBarIconHeight
                    text: "\ue5c4"
                    onClicked: {
                        fileChooser.goBack()
                    }
                }
                AddrBarArea {
                    id: pathBar
                    Layout.topMargin: 12
                    Layout.bottomMargin: 12
                    Layout.leftMargin: 0
                    Layout.rightMargin: 8
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    dir: fileChooser.currDir
                    onShouldOpenDir: p => openDir(p)
                }
            }

            function inWindowMoveArea(gpos: point): bool {
                var lpos = mapFromGlobal(gpos)
                // 顶部栏的上面一部分和窗口连在一起，所以认为顶部栏的上面也可以用于移动窗口
                // 如果不把这部分算做可用于移动窗口，体验不好
                return lpos.y < pathBar.y
            }
        }

        FileListArea {
            id: entriesArea
            fileChooser: fileChooser
            implicitWidth: root.width
            Layout.fillHeight: true
            onShouldOpenDir: p => root.openDir(p)
            onShouldOpenFile: p => root.openFile(p)
        }
    }

    MouseArea {
        anchors.fill: parent
        onPressed: e => {
                       pathBar.removeFocus()
                       e.accepted = false
                   }
    }
}
