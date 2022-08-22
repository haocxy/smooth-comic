import QtQuick
import QtQuick.Layouts

import myapp.Util

RowLayout {

    property int btnHMargin: 8
    property int btnVMargin: 12

    MyToolButton {
        // full screen
        Layout.topMargin: btnVMargin
        Layout.bottomMargin: btnVMargin
        Layout.leftMargin: btnHMargin
        Layout.rightMargin: btnHMargin
        Layout.fillHeight: true
        Layout.preferredWidth: height
        text: window.isFullScreen ? "\uf1cf" : "\uf1ce"
        onClicked: {
            if (window.isFullScreen) {
                Window.window.showNormal()
            } else {
                Window.window.showFullScreen()
            }
        }
    }

    MyToolButton {
        // min
        Layout.topMargin: btnVMargin
        Layout.bottomMargin: btnVMargin
        Layout.leftMargin: btnHMargin
        Layout.rightMargin: btnHMargin
        Layout.fillHeight: true
        Layout.preferredWidth: height
        visible: $engine.isWindowed
        text: "\ue15b"
        onClicked: Window.window.showMinimized()
    }

    MyToolButton {
        // max
        Layout.topMargin: btnVMargin
        Layout.bottomMargin: btnVMargin
        Layout.leftMargin: btnHMargin
        Layout.rightMargin: btnHMargin
        Layout.fillHeight: true
        Layout.preferredWidth: height
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

    MyToolButton {
        // close
        Layout.topMargin: btnVMargin
        Layout.bottomMargin: btnVMargin
        Layout.leftMargin: btnHMargin
        Layout.rightMargin: btnHMargin
        Layout.fillHeight: true
        Layout.preferredWidth: height
        visible: $engine.isWindowed
        text: "\ue5cd"
        onClicked: Window.window.close()
    }
}
