import QtQuick
import QtQuick.Controls

import myapp


Rectangle {

    required property string comicPath

    PageReader {
        anchors.fill: parent
        controller: idController
    }

    ControlArea {
        width: parent.width * 0.4
        height: parent.height * 0.8
        anchors.left: parent.left
        anchors.verticalCenter: parent.verticalCenter
        onClicked: idController.switchLeftPage()
    }

    ControlArea {
        width: parent.width * 0.4
        height: parent.height * 0.8
        anchors.right: parent.right
        anchors.verticalCenter: parent.verticalCenter
        onClicked: idController.switchRightPage()
    }

    Controller {
        id: idController
        guiEngine: $engine
        filePath: comicPath
    }
}
