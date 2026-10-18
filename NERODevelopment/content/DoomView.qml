import QtQuick 2.15
import QtQuick.Controls 2.15

Item {
    id: doomView
    anchors.fill: parent
    visible: true

    Component.onCompleted: doomController.startGame()

    Rectangle {
        anchors.fill: parent
        color: "black"
    }

    Image {
        id: doomFrame
        anchors.centerIn: parent

        width: {
            var scaleX = parent.width / 640;
            var scaleY = parent.height / 400;
            var scale = Math.min(scaleX, scaleY);
            return 640 * scale;
        }
        height: {
            var scaleX = parent.width / 640;
            var scaleY = parent.height / 400;
            var scale = Math.min(scaleX, scaleY);
            return 400 * scale;
        }

        source: doomController.running ? "image://doom/frame?" + doomController.frameCounter : ""

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

    Connections {
        target: buttons
        function onPressed(button) {
            doomController.press(button);
        }
    }

    Component.onDestruction: {
        if (doomController.running) {
            doomController.stopGame();
        }
    }
}
