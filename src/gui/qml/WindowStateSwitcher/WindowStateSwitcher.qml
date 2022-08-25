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
        Layout.topMargin: btnVMargin
        Layout.bottomMargin: btnVMargin
        Layout.leftMargin: btnHMargin
        Layout.rightMargin: btnHMargin
        Layout.fillHeight: true
        Layout.preferredWidth: height * heightFactor
        text: window.isFullScreen ? "\uf1cf" : "\uf1ce"
        bgColor: mouseOver ? hoverColor : btnBgColor
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
        Layout.topMargin: btnVMargin
        Layout.bottomMargin: btnVMargin
        Layout.leftMargin: btnHMargin
        Layout.rightMargin: btnHMargin
        Layout.fillHeight: true
        Layout.preferredWidth: height * heightFactor
        visible: $engine.isWindowed
        text: "\ue15b"
        bgColor: mouseOver ? hoverColor : btnBgColor
        onClicked: $window.showMinimized()
    }

    WindowButton {
        // max
        objectName: "windowMaxButton"
        Layout.topMargin: btnVMargin
        Layout.bottomMargin: btnVMargin
        Layout.leftMargin: btnHMargin
        Layout.rightMargin: btnHMargin
        Layout.fillHeight: true
        Layout.preferredWidth: height * heightFactor
        visible: $engine.isWindowed
        text: window.isMax ? "\ue3e0" : "\ue835"
        bgColor: mouseOver ? hoverColor : btnBgColor
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
        Layout.topMargin: btnVMargin
        Layout.bottomMargin: btnVMargin
        Layout.leftMargin: btnHMargin
        Layout.rightMargin: btnHMargin
        Layout.fillHeight: true
        Layout.preferredWidth: height * heightFactor
        visible: $engine.isWindowed
        text: "\ue5cd"
        bgColor: mouseOver ? hoverColor : btnBgColor
        onClicked: $window.close()
    }
}
