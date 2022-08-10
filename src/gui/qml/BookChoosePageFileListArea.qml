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

    function entryWidth(areaWidth, goodWidth) {
        if ($engine.isWindowed) {
            return Math.min(idWindow.width, goodWidth)
        } else {
            return Math.min(idWindow.width, areaWidth / Math.floor(areaWidth / goodWidth))
        }
    }


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

                property int goodAreaWidth: idWindow.width - idScrollBar.width

                property int entryWidth: idRoot.entryWidth(goodAreaWidth, goodWidth)

                implicitWidth: goodAreaWidth

                columns: Math.max(1, Math.floor(goodAreaWidth / entryWidth))

                rowSpacing: 0

                columnSpacing: 0

                Repeater {
                    model: idRoot.fileChooser.dirs
                    delegate: FileTreeEntry {
                        fileChooser: idRoot.fileChooser
                        Layout.fillHeight: true
                        Layout.alignment: Qt.AlignTop
                        implicitWidth: idDirsByGridLayout.entryWidth
                    }
                }
            }

            GridLayout {
                id: idFilesByGridLayout

                property int goodWidth: 150

                property int goodAreaWidth: idWindow.width - idScrollBar.width

                property int entryWidth: idRoot.entryWidth(goodAreaWidth, goodWidth)

                implicitWidth: goodAreaWidth

                columns: Math.max(1, Math.floor(goodAreaWidth / entryWidth))

                rowSpacing: 0

                columnSpacing: 0

                Repeater {
                    model: idRoot.fileChooser.files
                    delegate: FileTreeEntry {
                        fileChooser: idRoot.fileChooser
                        Layout.fillHeight: true
                        Layout.alignment: Qt.AlignTop
                        implicitWidth: idFilesByGridLayout.entryWidth
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
