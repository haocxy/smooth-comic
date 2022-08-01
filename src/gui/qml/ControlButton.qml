import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Button {
    property string buttonText
    property int span: 40
    flat: true
    width: span
    height: span
    Layout.preferredWidth: width
    Layout.preferredHeight: height
    Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
    Text {
        anchors.centerIn: parent
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        text: parent.buttonText
        width: parent.width
        height: parent.height
        font.family: "Material Icons"
        font.pixelSize: width * 0.8
        color: "#235a9f"
    }
}
