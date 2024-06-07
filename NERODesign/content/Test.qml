import QtQuick 2.5
import QtQuick.Window 2.2

Window {
    visible: true
    width: 800
    height: 600

    Rectangle {
        id: origin
        x: 50
        y: 100
        width: 5
        height: width
        color: "blue"
        radius: width / 2
    }

    Rectangle {
        id: end
        x: 550
        y: 300
        width: 5
        height: width
        color: "red"
        radius: width / 2
    }

    Arrow {
        origin: origin
        end: end
        duration: 100
        color: "green"

        Component.onCompleted: {
            start()
        }
    }

    MouseArea {
        anchors.fill: parent
        onClicked: {
            Qt.quit()
        }
    }
}
