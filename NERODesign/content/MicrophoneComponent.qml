
import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Studio.Components
import Qt5Compat.GraphicalEffects

Item {
    width: 400
    height: 300
    property bool isTalking: false

    SequentialAnimation {
        loops: Animation.Infinite
        running: isTalking

        PropertyAnimation {
            target: overlay
            property: "color"
            to: "#00AA00"
            duration: 1000 // Animation duration in milliseconds
        }
        PropertyAnimation {
            target: overlay
            property: "color"
            to: "#00FF00"
            duration: 1000
        }

    }

    Image {
        id: microphone
        anchors.centerIn: parent
        height: parent.height
        width: parent.width
        visible: isTalking
        source: "images/microphone.svg"
        fillMode: Image.PreserveAspectFit
    }

    ColorOverlay{
        id: overlay
        anchors.fill: microphone
        source:microphone
        color:"transparent"
        transform:rotation
        antialiasing: true
    }
}
