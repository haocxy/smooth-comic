import QtQuick
import QtQuick.Layouts

import "../Util"

RowLayout {
    required property color btnBgColor

    property int btnHMargin: 4
    property int btnVMargin: 0
    property real heightFactor: 1.5
    property color hoverColor: btnBgColor.lighter(1.3)

    WindowButton {
        // full screen
        text: window.isFullScreen ? "\uf1cf" : "\uf1ce"
        onClicked: {
            if (window.isFullScreen) {
                $window.showNormal()
            } else {
                $window.showFullScreen()
            }
        }
    }

    WindowButton {
        // min
        visible: $engine.isWindowed
        text: "\ue15b"
        bgColor: mouseOver ? hoverColor : btnBgColor
        onClicked: $window.showMinimized()
    }

    WindowButton {
        // max
        objectName: "windowMaxButton"
        visible: $engine.isWindowed
        text: window.isMax ? "\ue3e0" : "\ue835"
        onClicked: {
            if (window.isMax) {
                $window.showNormal()
            } else {
                $window.showMaximized()
            }
        }
    }

    WindowButton {
        // close
        visible: $engine.isWindowed
        text: "\ue5cd"
        onClicked: $window.close()
    }
}
