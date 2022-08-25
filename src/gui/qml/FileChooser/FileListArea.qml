import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Qt.labs.platform

Flickable {
    id: root
    required property var fileChooser
    property int entryUiItemCount: entries.children.length - 1
    property int entryGap: 8
    property int viewType: EntryStyle.eIcon

    contentWidth: entries.width
    contentHeight: entries.height
    flickableDirection: Flickable.VerticalFlick

    z: -100

    signal shouldOpenDir(string path)

    signal shouldOpenFile(string path)

    topMargin: entryGap

    bottomMargin: entryGap

    leftMargin: $engine.isWindowed ? entryGap : scrollBar.width

    rightMargin: $engine.isWindowed ? entryGap : 0

    GridLayout {
        id: entries

        implicitWidth: helper.areaWidth

        columns: helper.columnCount

        rowSpacing: entryGap

        columnSpacing: entryGap

        Repeater {
            model: fileChooser.entries
            delegate: FsEntry {
                Layout.fillHeight: true
                Layout.alignment: Qt.AlignTop
                implicitWidth: helper.entryWidth
                onShouldOpenDir: p => root.shouldOpenDir(p)
                onShouldOpenFile: p => root.shouldOpenFile(p)
            }
        }
    }

    ScrollBar.vertical: ScrollBar {
        id: scrollBar
        policy: $engine.isWindowed ? ScrollBar.AlwaysOn : ScrollBar.AsNeeded
        width: $engine.isWindowed ? 16 : entryGap
    }

    QtObject {
        id: helper
        property int goodEntryWidth: $engine.isWindowed ? 300 : 200
        property int usableWidth: root.width - scrollBar.width - root.leftMargin - root.rightMargin
        property int areaWidth: calcAreaWidth()
        property int entryGapPairWidth: calcEntryGapPairWidth()
        property int columnCount: Math.max(1, Math.floor((areaWidth + entryGap) / entryGapPairWidth))
        property int entryWidth: columnCount > 1 ? entryGapPairWidth - entryGap : usableWidth

        function calcEntryGapPairWidth() {
            var vRootWidth = root.width + entryGap
            if ($engine.isWindowed) {
                return Math.min(vRootWidth, goodEntryWidth + entryGap)
            } else {
                var vAreaWidth = areaWidth + entryGap
                var vGoodEntryWidth = goodEntryWidth + entryGap
                return Math.min(vRootWidth, vAreaWidth / Math.floor(vAreaWidth / vGoodEntryWidth))
            }
        }

        function calcAreaWidth() {
            if ($engine.isWindowed) {
                var vUsable = usableWidth + entryGap
                var vGoodEntryWidth = goodEntryWidth + entryGap
                var cols = Math.floor(vUsable / vGoodEntryWidth)
                var areaWidth = vGoodEntryWidth * cols - entryGap
                return areaWidth
            } else {
                return usableWidth
            }
        }
    }
}
