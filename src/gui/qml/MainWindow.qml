import QtQuick
import QtQuick.Controls


Window{
    id: window
    visible: true
    width: 800
    height: 600

    StackView {
        id: stack
        initialItem: bookChoosePage
        anchors.fill: parent
    }

    Component {
        id: bookChoosePage
        BookChoosePage {
            width: window.width
            height: window.height
        }
    }

    Component {
        id: bookReadPage
        BookReadPage {
            width: window.width
            height: window.height
        }
    }

}
