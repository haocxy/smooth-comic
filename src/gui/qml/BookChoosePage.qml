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

        BookChoosePageTopArea {
            id: idTopArea
        }

        Flickable {
            implicitWidth: idWindow.width
            Layout.fillHeight: true
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
                Flow {
                    id: idFilesByFlow
                    property int goodWidth: 100
                    width: idWindow.width - idScrollBar.width

                    Repeater {
                        model: idFileChooser.entries
                        delegate: FileTreeEntry {
                            fileChooser: idFileChooser
                            Layout.alignment: Qt.AlignTop
                            width: Math.min(idWindow.width - idScrollBar.width, idFilesByFlow.goodWidth)
                        }
                    }
                }
            }

            Component {
                id: idCompFilesByGridLayout
                GridLayout {
                    id: idGridLayout
                    property int goodWidth: 100
                    implicitWidth: idWindow.width - idScrollBar.width
                    columns: Math.max(1, Math.floor(idGridLayout.width / idGridLayout.goodWidth))
                    rowSpacing: 0
                    columnSpacing: 0

                    Repeater {
                        model: idFileChooser.entries
                        delegate: FileTreeEntry {
                            fileChooser: idFileChooser
                            Layout.alignment: Qt.AlignTop
                            implicitWidth: Math.min(idWindow.width, idGridLayout.width / Math.floor(idGridLayout.width / idGridLayout.goodWidth))
                        }
                    }
                }
            }

            MouseArea {
                anchors.fill: parent
                onPressed: e => {
                    idTopArea.removeFocus()
                    e.accepted = false
                }
            }

            ScrollBar.vertical: ScrollBar {
                id: idScrollBar
                policy: $engine.isWindowed ? ScrollBar.AlwaysOn : ScrollBar.AsNeeded
            }
        }
    }
}
