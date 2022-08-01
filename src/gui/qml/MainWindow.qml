import QtQuick
import QtQuick.Controls


Window{
    id: window
    visible: true
    width: 800
    height: 600

    StackView {
        id: stack
        initialItem: bookReadPage
        anchors.fill: parent
    }

    Component {
        id: bookReadPage
        BookReadPage {
            width: window.width
            height: window.height
        }
    }

}
