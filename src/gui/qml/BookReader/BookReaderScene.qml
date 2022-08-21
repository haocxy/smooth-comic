import QtQuick
import QtQuick.Controls

import myapp


Rectangle {

    required property string comicPath



//    Rectangle{
//        anchors.centerIn: parent
//        height: 100
//        width: 100
//        color: "blue"
//    }

    PageReader {
        anchors.fill: parent
        controller: idController
    }

    Rectangle {
        width: parent.width * 0.2
        height: parent.height * 0.8
        anchors.left: parent.left
        anchors.verticalCenter: parent.verticalCenter
        color: "blue"
        opacity: 0.5

        MouseArea {
            anchors.fill: parent
            onPressed: idController.switchLeftPage()
        }
    }

//    ControlArea {
//        isLandscape: window.width >= window.height
//        width: isLandscape ? span : parent.width
//        height: isLandscape ? parent.height : span
//        x: isLandscape ? parent.width - span : 0
//        y: isLandscape ? 0 : parent.height - span
//    }

    Controller {
        id: idController
        guiEngine: $engine
        filePath: comicPath
    }
}
