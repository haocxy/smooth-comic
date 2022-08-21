import QtQuick
import QtQuick.Controls

import myapp


Rectangle {

    required property string comicPath

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

    Controller {
        id: idController
        guiEngine: $engine
        filePath: comicPath
    }
}
