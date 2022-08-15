import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Rectangle {
    id: idRoot
    property string dir

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
        text: idRoot.dir
        focus: false
        onEditingFinished: {
            Qt.inputMethod.hide()
            idRoot.dir = text
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
