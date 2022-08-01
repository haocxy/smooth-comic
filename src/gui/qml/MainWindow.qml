import QtQuick
import QtQuick.Controls


Window{
    id: window
    visible: true
    width: 800
    height: 600

    Rectangle{
        anchors.centerIn: parent
        height: 100
        width: 100
        color: "blue"
    }

    ToolArea {
        width: window.width
        height: 100
        y: window.height - height
    }
}