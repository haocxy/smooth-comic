import QtQuick
import QtQuick.Layouts

import myapp.Util

RowLayout {

    MyToolButton {
        // full screen
        Layout.topMargin: 12
        Layout.bottomMargin: 12
        Layout.leftMargin: 8
        Layout.rightMargin: 8
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
        Layout.topMargin: 12
        Layout.bottomMargin: 12
        Layout.leftMargin: 8
        Layout.rightMargin: 8
        Layout.fillHeight: true
        Layout.preferredWidth: height
        visible: $engine.isWindowed
        text: "\ue15b"
        onClicked: Window.window.showMinimized()
    }

    MyToolButton {
        // max
        Layout.topMargin: 12
        Layout.bottomMargin: 12
        Layout.leftMargin: 8
        Layout.rightMargin: 8
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
        Layout.topMargin: 12
        Layout.bottomMargin: 12
        Layout.leftMargin: 8
        Layout.rightMargin: 8
        Layout.fillHeight: true
        Layout.preferredWidth: height
        visible: $engine.isWindowed
        text: "\ue5cd"
        onClicked: Window.window.close()
    }
}
