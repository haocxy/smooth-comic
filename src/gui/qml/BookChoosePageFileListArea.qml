import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Qt.labs.platform

Flickable {
    id: idRoot
    required property var fileChooser

    contentWidth: idFiles.width
    contentHeight: idFiles.height
    flickableDirection: Flickable.VerticalFlick
    z: 1
    clip: true

    Loader {
        id: idFiles
        // 根据是否是窗口化平台，选择布局方式
        // 对于窗口化平台，因为窗口宽度随时可变，所以固定元素宽度，以保证调整窗口大小的过程中的视觉舒适
        // 对于非窗口化平台，因为窗口宽度只会在调整屏幕方向时改变，所以尽可能占满宽度
        sourceComponent: $engine.isWindowed ? idCompFilesByFlow : idCompFilesByGridLayout
    }

    Component {
        id: idCompFilesByFlow
        Column {
            anchors.fill: parent
            Flow {
                id: idDirsByFlow
                property int goodWidth: 100
                width: idWindow.width - idScrollBar.width

                Repeater {
                    model: idRoot.fileChooser.dirs
                    delegate: FileTreeEntry {
                        fileChooser: idRoot.fileChooser
                        Layout.alignment: Qt.AlignTop
                        width: Math.min(idWindow.width - idScrollBar.width,
                                        idDirsByFlow.goodWidth)
                    }
                }
            }

            Flow {
                id: idFilesByFlow
                property int goodWidth: 100
                width: idWindow.width - idScrollBar.width

                Repeater {
                    model: idRoot.fileChooser.files
                    delegate: FileTreeEntry {
                        fileChooser: idRoot.fileChooser
                        Layout.alignment: Qt.AlignTop
                        width: Math.min(idWindow.width - idScrollBar.width,
                                        idFilesByFlow.goodWidth)
                    }
                }
            }
        }
    }

    Component {
        id: idCompFilesByGridLayout
        Column {
            GridLayout {
                id: idDirsByGridLayout
                property int goodWidth: 150
                implicitWidth: idWindow.width - idScrollBar.width
                columns: Math.max(1, Math.floor(
                                      idDirsByGridLayout.width / idDirsByGridLayout.goodWidth))
                rowSpacing: 0
                columnSpacing: 0

                Repeater {
                    model: idRoot.fileChooser.dirs
                    delegate: FileTreeEntry {
                        fileChooser: idRoot.fileChooser
                        Layout.fillHeight: true
                        Layout.alignment: Qt.AlignTop
                        implicitWidth: Math.min(
                                           idWindow.width,
                                           idDirsByGridLayout.width / Math.floor(
                                               idDirsByGridLayout.width / idDirsByGridLayout.goodWidth))
                    }
                }
            }

            GridLayout {
                id: idFilesByGridLayout
                property int goodWidth: 150
                implicitWidth: idWindow.width - idScrollBar.width
                columns: Math.max(1, Math.floor(
                                      idFilesByGridLayout.width / idFilesByGridLayout.goodWidth))
                rowSpacing: 0
                columnSpacing: 0

                Repeater {
                    model: idRoot.fileChooser.files
                    delegate: FileTreeEntry {
                        fileChooser: idRoot.fileChooser
                        Layout.fillHeight: true
                        Layout.alignment: Qt.AlignTop
                        implicitWidth: Math.min(
                                           idWindow.width,
                                           idFilesByGridLayout.width / Math.floor(
                                               idFilesByGridLayout.width / idFilesByGridLayout.goodWidth))
                    }
                }
            }
        }
    }

    ScrollBar.vertical: ScrollBar {
        id: idScrollBar
        policy: $engine.isWindowed ? ScrollBar.AlwaysOn : ScrollBar.AsNeeded
    }
}
