import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Rectangle {
    id: idRoot

    function setDir(dir) {
        idCurrDir.text = dir
    }

    signal shouldOpenDir(string dir)

    function removeFocus() {
        idCurrDir.readOnly = true
        idCurrDir.focus = false
    }

    MouseArea {
        anchors.fill: parent
        onPressed: e => {
                       var gpos = mapToGlobal(e.x, e.y)
                       if (!idCurrDir.contains(idCurrDir.mapFromGlobal(
                                                   gpos.x, gpos.y))) {
                           removeFocus()
                       }
                       e.accepted = false
                   }
    }

    Connections {
        target: Qt.inputMethod
        function onVisibleChanged() {
            if (!target.visible) {
                removeFocus()
            }
        }
    }

    color: idCurrDir.focus ? idWindow.color : "#f5f5f5"

    radius: 8

    TextInput {
        id: idCurrDir
        anchors.fill: parent
        clip: true
        leftPadding: 8
        rightPadding: 8
        verticalAlignment: TextInput.AlignVCenter
        focus: false
        property string lastFinished: ""
        onEditingFinished: {
            Qt.inputMethod.hide()
            if (text !== lastFinished) {
                shouldOpenDir(text)
            }
            lastFinished = text
            focus = false
        }

        MouseArea {
            anchors.fill: parent
            onPressed: e => {
                           parent.readOnly = false
                           window.openInputMethod(idCurrDir)
                           e.accepted = true
                       }
        }
    }
}
