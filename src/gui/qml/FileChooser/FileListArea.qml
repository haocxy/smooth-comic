import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Qt.labs.platform

Flickable {
    id: idRoot
    required property var fileChooser
    property int entryUiItemCount: idDirsByGridLayout.children.length + idFilesByGridLayout.children.length - 2
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
            property int goodEntryWidth: 200
            property int areaWidth: calcAreaWidth()
            property int entryGapPairWidth: calcEntryGapPairWidth()
            property int entryWidth: entryGapPairWidth - entryGap
            property int columnCount: Math.max(1, Math.floor((areaWidth + entryGap) / entryGapPairWidth))

            function calcEntryGapPairWidth() {
                var vRootWidth = idRoot.width + entryGap
                if ($engine.isWindowed) {
                    return Math.min(vRootWidth, goodEntryWidth + entryGap)
                } else {
                    var vAreaWidth = areaWidth + entryGap
                    var vGoodEntryWidth = goodEntryWidth + entryGap
                    return Math.min(vRootWidth, vAreaWidth / Math.floor(vAreaWidth / vGoodEntryWidth))
                }
            }

            function calcAreaWidth() {
                var usable = idRoot.width
                        - idScrollBar.width
                        - idEntryList.leftPadding
                        - idEntryList.rightPadding

                if ($engine.isWindowed) {
                    var vUsable = usable + entryGap
                    var vGoodEntryWidth = goodEntryWidth + entryGap
                    var cols = Math.floor(vUsable / vGoodEntryWidth)
                    var areaWidth = vGoodEntryWidth * cols - entryGap
                    return areaWidth
                } else {
                    return usable
                }
            }
        }

        GridLayout {
            id: idDirsByGridLayout

            implicitWidth: helper.areaWidth

            columns: helper.columnCount

            rowSpacing: entryGap

            columnSpacing: entryGap

            Repeater {
                model: fileChooser.dirs
                delegate: FsEntry {
                    Layout.fillHeight: true
                    Layout.alignment: Qt.AlignTop
                    implicitWidth: helper.entryWidth
                    onClicked: p => idRoot.shouldOpenDir(p)
                }
            }
        }

        Rectangle {
            width: entryGap
            height: entryGap
        }

        GridLayout {
            id: idFilesByGridLayout

            implicitWidth: helper.areaWidth

            columns: helper.columnCount

            rowSpacing: entryGap

            columnSpacing: entryGap

            Repeater {
                model: fileChooser.files
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
