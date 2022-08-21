import QtQuick

Rectangle {
    id: idRoot
    property bool showArea: false
    color: "blue"
    opacity: showArea ? 0.5 : 0

    signal clicked

    MouseArea {
        anchors.fill: parent
        onClicked: idRoot.clicked()
    }
}
