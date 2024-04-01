import QtQuick 2.15
import QtQuick.Controls 2.15

Rectangle {
    id: spedometer
    property int dimension: 100
    property int maxSpeed: 100
    property int lowestSpeed: 0
    property int previousTopSpeed: 40
    property int currentSpeed: 50 //change to 0

    width: dimension
    height: dimension * 3
    radius: 5
    color: "black"
    border.color: "white"

    Text {
        id: maxSpeed
        text: spedometer.maxSpeed
        font.pixelSize: parent.width * 0.2
        color: "white"
        x: -(width + dimension / 20)
    }

    Text {
        id: lowestSpeed
        text: spedometer.lowestSpeed
        font.pixelSize: parent.width * 0.2
        color: "white"
        x: -(width + dimension / 20)
        y: spedometer.height - height
    }

    Rectangle {
        id: bar
        y: (spedometer.height - height) - 5
        anchors.horizontalCenter: parent.horizontalCenter
        width: topSpeedBar.width
        height: spedometer.height * percentageHeight - 10
        radius: 5
        property real percentageHeight: parseFloat(
                                            (spedometer.currentSpeed / spedometer.maxSpeed).toFixed(
                                                2)) // Calculate percentage of height relative to spedometer

        gradient: Gradient {
            GradientStop {
                position: -5 * (1 - bar.percentageHeight
                                + 0.1) // Start of gradient relative to parent's height
                color: "red"
            }
            GradientStop {
                position: 1.0 // End of gradient relative to parent's height
                color: "#55FF00"
            }
        }
        transformOrigin: Item.BottomLeft
        Behavior on rotation {
            SpringAnimation {
                spring: 1.4
                damping: .15
            }
        }
    }

    Text {
        id: topSpeed
        text: qsTr("TOP SPEED")
        color: "white"
        x: -(width + dimension / 20)
        y: spedometer.height * (1 - spedometer.previousTopSpeed / spedometer.maxSpeed)
        font.pixelSize: parent.width * 0.2
        Text {
            id: topSpeedNumber
            text: spedometer.previousTopSpeed + qsTr("MPH")
            color: "white"
            y: -height
            font.pixelSize: parent.width * 0.2

            x: (topSpeed.width - width)
        }
    }

    Rectangle {
        id: topSpeedBar
        y: spedometer.height * (1 - spedometer.previousTopSpeed / spedometer.maxSpeed)
        width: spedometer.width - dimension / 6
        anchors.horizontalCenter: parent.horizontalCenter
        height: 2
        color: "white"
    }

    function setCurrentSpeed(speed) {
        spedometer.currentSpeed = speed
    }

    function setTopSpeed(top_speed) {
        topSpeedNumber.text = top_speed + "MPH"
        topSpeed.y = top_speed
    }

    Behavior on currentSpeed {
        NumberAnimation {
            duration: 100
        }
    }
}
