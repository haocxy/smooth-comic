import QtQuick
import QtQuick.Controls
import QtQuick.Layouts


Item {
    id: root
    required property string name
    required property string path
    required property bool isDir
    property int entryStyle: EntryStyle.eCard
    implicitHeight: loader.implicitHeight

    signal shouldOpenDir(string path)

    signal shouldOpenFile(string path)

    Loader {
        id: loader
        sourceComponent: compCardStyledEntry
        width: parent.width
        height: parent.height
    }

    MouseArea {
        anchors.fill: parent
        onClicked: {
            if (root.isDir) {
                root.shouldOpenDir(root.path)
            } else {
                root.shouldOpenFile(root.path)
            }
        }
    }

    Component {
        id: compCardStyledEntry
        Rectangle {
            property int iconWidth: 64
            property int iconHeight: 72
            implicitHeight: Math.max(iconHeight + 24, entryName.contentHeight + 32)
            color: "white"
            radius: 8

            border {
                color: "#c8c8c8"
                width: 1
            }

            RowLayout {

                anchors.fill: parent

                Image {
                    Layout.topMargin: 16
                    Layout.bottomMargin: 16
                    Layout.leftMargin: 8
                    Layout.rightMargin: 8
                    Layout.preferredWidth: iconWidth
                    Layout.fillHeight: true
                    horizontalAlignment: Image.AlignHCenter
                    verticalAlignment: Image.AlignTop
                    source: isDir ? "/icon/folder" : "/icon/file"
                    asynchronous: true
                    sourceSize.width: width * Screen.devicePixelRatio
                    sourceSize.height: height * Screen.devicePixelRatio
                    fillMode: Image.PreserveAspectFit
                }

                Text {
                    Layout.topMargin: 16
                    Layout.bottomMargin: 16
                    Layout.leftMargin: 0
                    Layout.rightMargin: 16
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    id: entryName
                    padding: 0
                    antialiasing: true
                    text: root.name
                    wrapMode: Text.Wrap
                    font.pixelSize: 18
                }

            }
        }
    }

}
