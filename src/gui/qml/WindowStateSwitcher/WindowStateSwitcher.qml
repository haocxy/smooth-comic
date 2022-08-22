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
                Window.window.showNormal()
            } else {
                Window.window.showFullScreen()
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
        onClicked: Window.window.showMinimized()
    }

    WindowButton {
        // max
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
                Window.window.showNormal()
            } else {
                Window.window.showMaximized()
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
        onClicked: Window.window.close()
    }
}
