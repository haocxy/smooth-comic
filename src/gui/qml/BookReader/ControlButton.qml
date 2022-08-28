import QtQuick
import QtQuick.Controls

import "../Global"

ToolButton {
    required property int iconHeight
    background: null
    contentItem: Text {
        font {
            family: "Material Icons"
            pixelSize: iconHeight
        }
        color: Style.iconColor
        text: parent.text
    }
}
