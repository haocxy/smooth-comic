import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Rectangle {
    property int goodHeight: idCurrDir.contentHeight + 36
    color: "#c8c8c8"
    z: 1

    function removeFocus() {
        idCurrDir.readOnly = true
        idCurrDir.focus = false
    }

    MouseArea {
        anchors.fill: parent
        onPressed: e => {
            var gpos = mapToGlobal(e.x, e.y)
            if (!idCurrDir.contains(idCurrDir.mapFromGlobal(gpos.x, gpos.y))) {
                removeFocus()
            }
            e.accepted = false
        }
    }

    RowLayout {
        anchors.fill: parent
        spacing: 8

        Rectangle {
            Layout.fillWidth: true
            Layout.preferredHeight: 36
            Layout.margins: 8

            color: idCurrDir.focus ? idWindow.color : "#f5f5f5"
            width: parent.width * 0.6

            radius: 4

            TextInput {
                id: idCurrDir
                anchors.fill: parent
                clip: true
                leftPadding: 8
                rightPadding: 8
                verticalAlignment: TextInput.AlignVCenter
                text: idFileChooser.currDir
                focus: false
                onEditingFinished: {
                    Qt.inputMethod.hide()
                    idFileChooser.currDir = text
                    focus = false
                }

                MouseArea {
                    anchors.fill: parent
                    onPressed: e => {
                                   parent.readOnly = false
                                   e.accepted = false
                               }
                }
            }
        }
    }
}
