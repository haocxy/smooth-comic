import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Rectangle {
    property string dir

    signal shouldOpenDir(string dir)

    function removeFocus() {
        pathInput.readOnly = true
        pathInput.focus = false
    }

    MouseArea {
        anchors.fill: parent
        onPressed: e => {
                       var gpos = mapToGlobal(e.x, e.y)
                       if (!pathInput.contains(pathInput.mapFromGlobal(
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

    radius: 8

    TextInput {
        id: pathInput
        anchors.fill: parent
        clip: true
        text: dir
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
                           window.openInputMethod(pathInput)
                           e.accepted = true
                       }
        }
    }
}
