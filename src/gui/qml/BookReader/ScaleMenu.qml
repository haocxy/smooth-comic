import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Popup {
    required property var controller
    width: 300
    height: 100
    modal: true

    ButtonGroup {
        id: idButtonGroup
    }

    ColumnLayout {
        anchors.fill: parent
        RowLayout {
            Item {
                Layout.fillWidth: true
                Layout.fillHeight: true
            }
            RadioButton {
                ButtonGroup.group: idButtonGroup
                text: qsTr("Raw Size")
            }
            RadioButton {
                ButtonGroup.group: idButtonGroup
                text: qsTr("Adjust Size")
            }
        }
        RowLayout {
            Item {
                Layout.fillWidth: true
                Layout.fillHeight: true
            }
            RadioButton {
                ButtonGroup.group: idButtonGroup
                text: qsTr("Raw Size")
            }
            RadioButton {
                ButtonGroup.group: idButtonGroup
                text: qsTr("Adjust Size")
            }
        }
        Item {
            Layout.fillWidth: true
            Layout.fillHeight: true
        }
    }
}
