import QtQuick 2.15
import QtQuick.Controls 2.15

Item {
    id: doomView
    anchors.fill: parent
    focus: doomView.isFocused
    visible: true

    property bool isFocused: false

    onIsFocusedChanged: {
        if (isFocused) {
            if (!doomController.running) {
                doomController.startGame()
            }
        } else {
            if (doomController.running) {
                doomController.stopGame()
            }
        }
    }

    Rectangle {
        anchors.fill: parent
        color: "black"
    }

    Image {
        id: doomFrame
        anchors.centerIn: parent

        width: {
            var scaleX = parent.width / 320
            var scaleY = parent.height / 200
            var scale = Math.min(scaleX, scaleY)
            return 320 * scale
        }
        height: {
            var scaleX = parent.width / 320
            var scaleY = parent.height / 200
            var scale = Math.min(scaleX, scaleY)
            return 200 * scale
        }

        source: doomController.running
                ? "image://doom/frame?" + doomController.frameCounter
                : ""

        smooth: false
        fillMode: Image.Stretch
        cache: false
        visible: doomController.running
    }

    Keys.onPressed: function(event) {
        if (event.isAutoRepeat) return

        switch (event.key) {
            case Qt.Key_Up:
                doomController.sendKey(0xad, true)
                event.accepted = true
                break
            case Qt.Key_Down:
                doomController.sendKey(0xaf, true)
                event.accepted = true
                break
            case Qt.Key_Left:
                doomController.sendKey(0xac, true)
                event.accepted = true
                break
            case Qt.Key_Right:
                doomController.sendKey(0xae, true)
                event.accepted = true
                break
            case Qt.Key_Return:
                if (!doomController.running) {
                    doomController.startGame()
                } else {
                    doomController.sendKey(0x0d, true)
                    doomController.sendKey(0x9d, true)
                    doomController.sendKey(0x20, true)
                }
                event.accepted = true
                break
            case Qt.Key_Escape:
                if (doomController.running) {
                    doomController.stopGame()
                }
                navigationController.goHome()
                event.accepted = true
                break
        }
    }

    Keys.onReleased: function(event) {
        if (event.isAutoRepeat) return

        switch (event.key) {
            case Qt.Key_Up:
                doomController.sendKey(0xad, false)
                event.accepted = true
                break
            case Qt.Key_Down:
                doomController.sendKey(0xaf, false)
                event.accepted = true
                break
            case Qt.Key_Left:
                doomController.sendKey(0xac, false)
                event.accepted = true
                break
            case Qt.Key_Right:
                doomController.sendKey(0xae, false)
                event.accepted = true
                break
            case Qt.Key_Return:
                doomController.sendKey(0x0d, false)
                doomController.sendKey(0x9d, false)
                doomController.sendKey(0x20, false)
                event.accepted = true
                break
        }
    }

    Component.onDestruction: {
        if (doomController.running) {
            doomController.stopGame()
        }
    }
}
