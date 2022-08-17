import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Qt.labs.platform

Flickable {
    id: idRoot
    required property var frame
    property int entryGap: 8
    property int viewType: EntryStyle.eIcon
    contentWidth: idEntryList.width
    contentHeight: idEntryList.height
    flickableDirection: Flickable.VerticalFlick
    z: 1
    clip: true

    signal shouldOpenDir(string path)

    signal shouldOpenFile(string path)

    function entryWidth(areaWidth, goodWidth) {
        if ($engine.isWindowed) {
            return Math.min(idWindow.width, goodWidth)
        } else {
            return Math.min(idWindow.width, areaWidth / Math.floor(areaWidth / goodWidth))
        }
    }

    Column {

        id: idEntryList

        padding: entryGap

        QtObject {
            id: helper
            property int goodEntryWidth: 150
            property int areaWidth: calcAreaWidth()
            property int entryWidth: idRoot.entryWidth(areaWidth, goodEntryWidth)
            property int columnCount: Math.max(1, Math.floor(areaWidth / entryWidth))

            function calcAreaWidth() {
                if ($engine.isWindowed) {
                    var cols = Math.floor((idRoot.width - idScrollBar.width - idEntryList.leftPadding - idEntryList.rightPadding) / goodEntryWidth)
                    return goodEntryWidth * cols
                } else {
                    return idRoot.width - idScrollBar.width - idEntryList.leftPadding - idEntryList.rightPadding
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
                model: idRoot.frame.dirs
                delegate: FsEntry {
                    Layout.fillHeight: true
                    Layout.alignment: Qt.AlignTop
                    implicitWidth: helper.entryWidth
                    onClicked: p => idRoot.shouldOpenDir(p)
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
                model: idRoot.frame.files
                delegate: FsEntry {
                    Layout.fillHeight: true
                    Layout.alignment: Qt.AlignTop
                    implicitWidth: helper.entryWidth
                    onClicked: p => idRoot.shouldOpenFile(p)
                }
            }
        }
    }

    ScrollBar.vertical: ScrollBar {
        id: idScrollBar
        policy: $engine.isWindowed ? ScrollBar.AlwaysOn : ScrollBar.AsNeeded
    }
}
