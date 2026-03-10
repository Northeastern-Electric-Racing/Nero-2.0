import QtQuick
import QtQuick.Controls
import Qt5Compat.GraphicalEffects
import NERO

Item {
    width: 100
    height: 100
    property bool isTalking: false

    SequentialAnimation {
        loops: Animation.Infinite
        running: isTalking

        PropertyAnimation {
            target: overlay
            property: "color"
            to: Theme.primaryMicrophone
            duration: 1000 // Animation duration in milliseconds
        }
        PropertyAnimation {
            target: overlay
            property: "color"
            to: Theme.secondaryMicrophone
            duration: 1000
        }
    }

    Image {
        id: microphone
        anchors.centerIn: parent
        height: parent.height
        width: parent.width
        visible: isTalking
        source: "/qt/qml/content/images/microphone.svg"
        fillMode: Image.PreserveAspectFit
    }

    ColorOverlay {
        id: overlay
        anchors.fill: microphone
        source: microphone
        color: Theme.background
        transform: rotation
        antialiasing: true
    }
}
