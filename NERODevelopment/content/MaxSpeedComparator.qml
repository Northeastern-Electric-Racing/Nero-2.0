import QtQuick 2.15
import QtQuick.Controls 2.15

Rectangle {
    id: maxSpeedComparator
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
        text: maxSpeedComparator.maxSpeed
        font.pixelSize: parent.width * 0.2
        color: "white"
        x: -(width + dimension / 20)
    }

    Text {
        id: lowestSpeed
        text: maxSpeedComparator.lowestSpeed
        font.pixelSize: parent.width * 0.2
        color: "white"
        x: -(width + dimension / 20)
        y: maxSpeedComparator.height - height
    }

    Rectangle {
        id: bar
        y: (maxSpeedComparator.height - height) - 5
        anchors.horizontalCenter: parent.horizontalCenter
        width: topSpeedBar.width
        height: maxSpeedComparator.height * percentageHeight - 10
        radius: 5
        property real percentageHeight: parseFloat(
                                            (maxSpeedComparator.currentSpeed
                                             / maxSpeedComparator.maxSpeed).toFixed(
                                                2)) // Calculate percentage of height relative to maxSpeedComparator

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
        y: maxSpeedComparator.height * (1 - maxSpeedComparator.previousTopSpeed
                                        / maxSpeedComparator.maxSpeed)
        font.pixelSize: parent.width * 0.2
        Text {
            id: topSpeedNumber
            text: maxSpeedComparator.previousTopSpeed + qsTr("MPH")
            color: "white"
            y: -height
            font.pixelSize: parent.width * 0.2

            x: (topSpeed.width - width)
        }
    }

    onCurrentSpeedChanged: {
        console.log("Speededd", maxSpeedComparator.currentSpeed)
    }

    Rectangle {
        id: topSpeedBar
        y: maxSpeedComparator.height * (1 - maxSpeedComparator.previousTopSpeed
                                        / maxSpeedComparator.maxSpeed)
        width: maxSpeedComparator.width - dimension / 6
        anchors.horizontalCenter: parent.horizontalCenter
        height: 2
        color: "white"
    }

    function setCurrentSpeed(speed) {
        maxSpeedComparator.currentSpeed = speed
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
