import QtQuick 2.15
import QtQuick.Controls 2.15

Item {
    id: doomView
    anchors.fill: parent
    focus: doomView.isFocused
    visible: true

    property bool isFocused: false

    readonly property int doomKeyUp:     0xAD
    readonly property int doomKeyDown:   0xAF
    readonly property int doomKeyLeft:   0xAC
    readonly property int doomKeyRight:  0xAE
    readonly property int doomKeyEnter:  0x0D
    readonly property int doomKeyUse:    0x9D
    readonly property int doomKeyFire:   0x20
    readonly property int doomKeyEscape: 0x1B

    // Hold to exit
    property bool escHeld: false

    Timer {
        id: escExitTimer
        interval: 1000
        repeat: false
        onTriggered: {
            doomView.escHeld = true
            doomController.sendKey(doomKeyEscape, false)
            if (doomController.running) {
                doomController.stopGame()
            }
            navigationController.goHome()
        }
    }

    onIsFocusedChanged: {
        if (isFocused) {
            if (!doomController.running) {
                doomController.startGame()
            }
        } else {
            escExitTimer.stop()
            escHeld = false
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
                doomController.sendKey(doomKeyUp, true)
                event.accepted = true
                break
            case Qt.Key_Down:
                doomController.sendKey(doomKeyDown, true)
                event.accepted = true
                break
            case Qt.Key_Left:
                doomController.sendKey(doomKeyLeft, true)
                event.accepted = true
                break
            case Qt.Key_Right:
                doomController.sendKey(doomKeyRight, true)
                event.accepted = true
                break
            case Qt.Key_Return:
                if (!doomController.running) {
                    doomController.startGame()
                } else {
                    doomController.sendKey(doomKeyEnter, true)
                    doomController.sendKey(doomKeyUse, true)
                    doomController.sendKey(doomKeyFire, true)
                }
                event.accepted = true
                break
            case Qt.Key_Escape:
                if (doomController.running) {
                    doomController.sendKey(doomKeyEscape, true)
                    escExitTimer.start()
                } else {
                    navigationController.goHome()
                }
                event.accepted = true
                break
        }
    }

    Keys.onReleased: function(event) {
        if (event.isAutoRepeat) return

        switch (event.key) {
            case Qt.Key_Up:
                doomController.sendKey(doomKeyUp, false)
                event.accepted = true
                break
            case Qt.Key_Down:
                doomController.sendKey(doomKeyDown, false)
                event.accepted = true
                break
            case Qt.Key_Left:
                doomController.sendKey(doomKeyLeft, false)
                event.accepted = true
                break
            case Qt.Key_Right:
                doomController.sendKey(doomKeyRight, false)
                event.accepted = true
                break
            case Qt.Key_Return:
                doomController.sendKey(doomKeyEnter, false)
                doomController.sendKey(doomKeyUse, false)
                doomController.sendKey(doomKeyFire, false)
                event.accepted = true
                break
            case Qt.Key_Escape:
                escExitTimer.stop()
                if (!escHeld && doomController.running) {
                    doomController.sendKey(doomKeyEscape, false)
                }
                escHeld = false
                event.accepted = true
                break
        }
    }

    Component.onDestruction: {
        escExitTimer.stop()
        if (doomController.running) {
            doomController.stopGame()
        }
    }
}
