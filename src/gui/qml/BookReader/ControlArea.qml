import QtQuick

Rectangle {
    property bool showArea: false
    color: "blue"
    opacity: showArea ? 0.5 : 0

    signal triggered

    MouseArea {
        anchors.fill: parent
        onReleased: triggered()
    }
}
