import QtQuick
import QtQuick.Controls
import QtQuick.Layouts


Item {
    id: idEntry
    required property string name
    required property string path
    required property bool isDir
    property int entryStyle: EntryStyle.eCard
    implicitHeight: idLoader.implicitHeight

    signal clicked(string path)

    Loader {
        id: idLoader
        sourceComponent: idCompCardStyledEntry
        width: idEntry.width
        height: idEntry.height
    }

    MouseArea {
        anchors.fill: parent
        onClicked: {
            if (idEntry.isDir) {
                idEntry.clicked(idEntry.path)
            }
        }
    }

    Component {
        id: idCompCardStyledEntry
        Rectangle {
            property int iconWidth: 64
            property int iconHeight: 72
            implicitHeight: Math.max(iconHeight + 8, idFileName.contentHeight + idFileName.padding + 16)
            color: "white"
            radius: 4

            border {
                color: "#c8c8c8"
                width: 1
            }

            RowLayout {

                anchors.fill: parent

                Image {
                    Layout.preferredWidth: iconWidth
                    Layout.fillHeight: true
                    horizontalAlignment: Image.AlignHCenter
                    verticalAlignment: Image.AlignVCenter
                    source: isDir ? "/icon/folder" : "/icon/file-unknown"
                    asynchronous: true
                    sourceSize.width: width
                    sourceSize.height: height
                    fillMode: Image.PreserveAspectFit
                }

                Text {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    id: idFileName
                    padding: 16
                    antialiasing: true
                    text: idEntry.name
                    wrapMode: Text.Wrap
                    font.pixelSize: 18
                }

            }
        }
    }

}
