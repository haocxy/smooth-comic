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

    function updateFrames() {
        var frames = idFileChooser.stack.frames
        var frameCount = frames.length

        // 如果公共长度部分的栈帧存在不同，则说明有过老的栈帧被清除，应该把StackView中不同的部分弹出
        var commonCount = Math.min(frameCount, idDirStack.depth)
        for (var i = 0; i < commonCount; ++i) {
            if (frames[i].name !== idDirStack.get(i, StackView.DontLoad).name) {
                // 此时i为相同部分的长度，需要把后面的栈帧弹出
                while (i + 1 < idDirStack.depth) {
                    idDirStack.pop()
                }
                break
            }
        }

        // 添加新增的栈帧
        for (var frameIndex = 0; frameIndex < frameCount; ++frameIndex) {
            if (frameIndex > idDirStack.last) {
                idDirStack.push(idCompFileListArea.createObject(idDirStack, {frame: frames[frameIndex]}))
            }
        }

        // 删除已经被弹出的栈帧
        while (frameCount < idDirStack.depth) {
            idDirStack.pop()
        }
    }

    function openDir(path) {
        idFileChooser.openDir(path)
    }

    Component.onCompleted: idFileChooser.openInitDir()

    Connections {
        target: idFileChooser.stack
        function onFramesChanged() {
            updateFrames()
        }
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
                }
            }
        }

        StackView {
            id: idDirStack
            property int last: depth - 1
            Layout.fillWidth: true
            Layout.fillHeight: true
        }

        Component {
            id: idCompFileListArea
            FileListArea {
                implicitWidth: idWindow.width
                Layout.fillHeight: true
                onShouldOpenDir: p => idWindow.openDir(p)
            }
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
