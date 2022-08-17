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
    z: -100

    signal shouldOpenDir(string path)

    signal shouldOpenFile(string path)

    Column {

        id: idEntryList

        topPadding: entryGap
        bottomPadding: entryGap
        leftPadding: $engine.isWindowed ? entryGap : idScrollBar.width
        rightPadding: $engine.isWindowed ? entryGap : 0

        QtObject {
            id: helper
            property int goodEntryWidth: 150
            property int areaWidth: calcAreaWidth()
            property int entryWidth: calcEntryWidth()
            property int columnCount: Math.max(1, Math.floor(areaWidth / entryWidth))

            function calcEntryWidth() {
                if ($engine.isWindowed) {
                    return Math.min(idRoot.width, goodEntryWidth)
                } else {
                    return Math.min(idRoot.width, areaWidth / Math.floor(areaWidth / goodEntryWidth))
                }
            }

            function calcAreaWidth() {
                var usable = idRoot.width
                        - idScrollBar.width
                        - idEntryList.leftPadding
                        - idEntryList.rightPadding

                if ($engine.isWindowed) {
                    var cols = Math.floor(usable / goodEntryWidth)
                    return goodEntryWidth * cols
                } else {
                    return usable
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
        width: $engine.isWindowed ? 16 : entryGap
    }
}
