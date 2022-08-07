import QtQuick
import QtQuick.Controls

import myapp


Rectangle {

    Rectangle{
        anchors.centerIn: parent
        height: 100
        width: 100
        color: "blue"
    }

    ControlArea {
        isLandscape: window.width >= window.height
        width: isLandscape ? span : parent.width
        height: isLandscape ? parent.height : span
        x: isLandscape ? parent.width - span : 0
        y: isLandscape ? 0 : parent.height - span
    }

    Book {
        id: book
        guiEngine: $engine
    }
}