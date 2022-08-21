import QtQuick
import QtQuick.Controls

import myapp


Rectangle {

    required property string comicPath

    PageReader {
        anchors.fill: parent
        controller: idController
    }

    Item {
        id: idClickControlls
        anchors.fill: parent
        opacity: 0

        function controll(gpos) {
            var lpos = mapToItem(this, gpos)
            var item = childAt(lpos.x, lpos.y)
            if (!item) {
                return
            }
            if (item.handle) {
                item.handle()
            }
        }

        Rectangle {
            width: parent.width * (1 - 0.3 * 2)
            height: parent.height * ((1 - 0.6) / 2)
            opacity: 0.5
            color: "blue"
            anchors.top: parent.top
            anchors.horizontalCenter: parent.horizontalCenter

            function handle() {
                idController.switchUpPage()
            }
        }

        Rectangle {
            width: parent.width * (1 - 0.3 * 2)
            height: parent.height * ((1 - 0.6) / 2)
            opacity: 0.5
            color: "blue"
            anchors.bottom: parent.bottom
            anchors.horizontalCenter: parent.horizontalCenter

            function handle() {
                idController.switchDownPage()
            }
        }

        Rectangle {
            width: parent.width * 0.3
            height: parent.height * 0.6
            opacity: 0.5
            color: "blue"
            anchors.left: parent.left
            anchors.verticalCenter: parent.verticalCenter

            function handle() {
                idController.switchLeftPage()
            }
        }

        Rectangle {
            width: parent.width * 0.3
            height: parent.height * 0.6
            opacity: 0.5
            color: "blue"
            anchors.right: parent.right
            anchors.verticalCenter: parent.verticalCenter

            function handle() {
                idController.switchRightPage()
            }
        }

    }

    PinchArea {
        id: idPinchArea
        property real currScale: 1
        anchors.fill: parent

        onPinchUpdated: e => {
                            if (e.previousScale !== 0) {
                                idController.relativelyScale(e.scale / e.previousScale)
                            }
                        }

        MouseArea {
            id: idMouseArea
            anchors.fill: parent
            onClicked: e => {
                            if (!idMouseArea.moved) {
                                idClickControlls.controll(mapToGlobal(e.x, e.y))
                            }
                        }
            property real prevX: 0
            property real prevY: 0
            property bool moved: false
            onPressed: e => {
                           idMouseArea.prevX = e.x
                           idMouseArea.prevY = e.y
                           idMouseArea.moved = false
                       }

            onPositionChanged: e => {
                                   var dx = (e.x - prevX) * Screen.devicePixelRatio
                                   var dy = (e.y - prevY) * Screen.devicePixelRatio
                                   idController.translateBy(dx, dy)
                                   idMouseArea.prevX = e.x
                                   idMouseArea.prevY = e.y
                                   idMouseArea.moved = true
                               }
        }

    }



    Controller {
        id: idController
        guiEngine: $engine
        filePath: comicPath
    }
}
