import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Button {
    property string buttonText
    flat: true
    width: 80
    height: 80
    Layout.preferredWidth: width
    Layout.preferredHeight: height
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
