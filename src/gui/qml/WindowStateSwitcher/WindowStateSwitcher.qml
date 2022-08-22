import QtQuick
import QtQuick.Layouts

import myapp.Util

RowLayout {

    property int btnHMargin: 4
    property int btnVMargin: 0

    property real heightFactor: 1.5

    WindowButton {
        // full screen
        Layout.topMargin: btnVMargin
        Layout.bottomMargin: btnVMargin
        Layout.leftMargin: btnHMargin
        Layout.rightMargin: btnHMargin
        Layout.fillHeight: true
        Layout.preferredWidth: height * heightFactor
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
        Layout.topMargin: btnVMargin
        Layout.bottomMargin: btnVMargin
        Layout.leftMargin: btnHMargin
        Layout.rightMargin: btnHMargin
        Layout.fillHeight: true
        Layout.preferredWidth: height * heightFactor
        visible: $engine.isWindowed
        text: "\ue15b"
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
        onClicked: $window.close()
    }
}
