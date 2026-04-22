import QtQuick 2.15
import QtQuick.Controls 2.15

Item {
    id: doomView
    anchors.fill: parent
    visible: true

    // isFocused is set externally by the navigation system (NavigationController).
    // When this page becomes the active page, isFocused is set to true, granting
    // keyboard focus. This matches the pattern used by FlappyBird.qml and Snake.qml.
    property bool isFocused: false
    focus: doomView.isFocused

    // DOOM key codes from doomgeneric's doomkeys.h
    // (https://github.com/ozkl/doomgeneric/blob/master/doomgeneric/doomkeys.h)
    // These must match the values defined in the library — they are NOT arbitrary.
    readonly property int doomKeyUp:      0xAD  // KEY_UPARROW
    readonly property int doomKeyDown:    0xAF  // KEY_DOWNARROW
    readonly property int doomKeyLeft:    0xAC  // KEY_LEFTARROW
    readonly property int doomKeyRight:   0xAE  // KEY_RIGHTARROW
    readonly property int doomKeyEnter:   0x0D  // KEY_ENTER
    readonly property int doomKeyFire:    0x9D  // KEY_FIRE (ctrl)
    readonly property int doomKeyUse:     0x20  // KEY_USE (space — open doors, switches)
    readonly property int doomKeyEscape:  0x1B  // KEY_ESCAPE

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
            var scaleX = parent.width / 640
            var scaleY = parent.height / 400
            var scale = Math.min(scaleX, scaleY)
            return 640 * scale
        }
        height: {
            var scaleX = parent.width / 640
            var scaleY = parent.height / 400
            var scale = Math.min(scaleX, scaleY)
            return 400 * scale
        }

        source: doomController.running
                ? "image://doom/frame?" + doomController.frameCounter
                : ""

        // nearest-neighbor scaling preserves DOOM's original pixel art look
        smooth: false
        fillMode: Image.Stretch

        // cache must be disabled because we reuse the same base URI ("image://doom/frame")
        // and append a changing frameCounter query param to force QML to re-request.
        // With caching on, QML may serve a stale frame from its internal image cache
        // instead of calling DoomImageProvider::requestImage() for the latest frame.
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
                    doomController.sendKey(doomKeyFire, true)
                    doomController.sendKey(doomKeyUse, true)
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
                doomController.sendKey(doomKeyFire, false)
                doomController.sendKey(doomKeyUse, false)
                event.accepted = true
                break
        }
    }

    // When the hardware escape button (button 1) is pressed,
    // DoomController emits escapeRequested after stopping DOOM.
    // This navigates back to the home screen.
    Connections {
        target: doomController
        function onEscapeRequested() {
            navigationController.goHome()
        }
    }

    Component.onDestruction: {
        if (doomController.running) {
            doomController.stopGame()
        }
    }
}
