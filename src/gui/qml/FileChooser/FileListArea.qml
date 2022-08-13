import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Qt.labs.platform

Flickable {
    id: idRoot
    required property var fileChooser
    contentWidth: idEntryList.width
    contentHeight: idEntryList.height
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

    Connections {
        target: fileChooser
        function onSigRestoreContentY(restoredContentY) {
            idRoot.contentY = restoredContentY
        }
    }

    Column {

        id: idEntryList

        QtObject {
            id: helper
            property int goodEntryWidth: 150
            property int areaWidth: calcAreaWidth()
            property int entryWidth: idRoot.entryWidth(areaWidth, goodEntryWidth)
            property int columnCount: Math.max(1, Math.floor(areaWidth / entryWidth))

            function calcAreaWidth() {
                if ($engine.isWindowed) {
                    var cols = Math.floor((idWindow.width - idScrollBar.width) / goodEntryWidth)
                    return goodEntryWidth * cols
                } else {
                    return idWindow.width - idScrollBar.width
                }
            }
        }

        GridLayout {
            id: idDirsByGridLayout

            implicitWidth: helper.areaWidth

            columns: helper.columnCount

            rowSpacing: 0

            columnSpacing: 0

            Repeater {
                model: idRoot.fileChooser.dirs
                delegate: FsEntry {
                    fileChooser: idRoot.fileChooser
                    fileListFlickable: idRoot
                    Layout.fillHeight: true
                    Layout.alignment: Qt.AlignTop
                    implicitWidth: helper.entryWidth
                }
            }
        }

        GridLayout {
            id: idFilesByGridLayout

            implicitWidth: helper.areaWidth

            columns: helper.columnCount

            rowSpacing: 0

            columnSpacing: 0

            Repeater {
                model: idRoot.fileChooser.files
                delegate: FsEntry {
                    fileChooser: idRoot.fileChooser
                    fileListFlickable: idRoot
                    Layout.fillHeight: true
                    Layout.alignment: Qt.AlignTop
                    implicitWidth: helper.entryWidth
                }
            }
        }
    }

    ScrollBar.vertical: ScrollBar {
        id: idScrollBar
        policy: $engine.isWindowed ? ScrollBar.AlwaysOn : ScrollBar.AsNeeded
    }
}
