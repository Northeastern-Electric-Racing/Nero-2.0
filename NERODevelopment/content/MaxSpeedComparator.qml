import QtQuick
import QtQuick.Controls
import NERO

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
    color: Theme.getColor("background")
    border.color: Theme.getColor("primaryForeground")

    Text {
        id: maxSpeed
        text: maxSpeedComparator.maxSpeed
        font.pixelSize: parent.width * 0.2
        color: Theme.getColor("primaryForeground")
        x: -(width + dimension / 20)
    }

    Text {
        id: lowestSpeed
        text: maxSpeedComparator.lowestSpeed
        font.pixelSize: parent.width * 0.2
        color: Theme.getColor("primaryForeground")
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
                color: Theme.getColor("criticalStatus")
            }
            GradientStop {
                position: 1.0 // End of gradient relative to parent's height
                color: Theme.getColor("goodStatus")
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
        color: Theme.getColor("primaryForeground")
        x: -(width + dimension / 20)
        y: maxSpeedComparator.height * (1 - maxSpeedComparator.previousTopSpeed
                                        / maxSpeedComparator.maxSpeed)
        font.pixelSize: parent.width * 0.2
        Text {
            id: topSpeedNumber
            text: maxSpeedComparator.previousTopSpeed + qsTr("MPH")
            color: Theme.getColor("primaryForeground")
            y: -height
            font.pixelSize: parent.width * 0.2

            x: (topSpeed.width - width)
        }
    }

    Rectangle {
        id: topSpeedBar
        y: maxSpeedComparator.height * (1 - maxSpeedComparator.previousTopSpeed
                                        / maxSpeedComparator.maxSpeed)
        width: maxSpeedComparator.width - dimension / 6
        anchors.horizontalCenter: parent.horizontalCenter
        height: 2
        color: Theme.getColor("primaryForeground")
    }

    Behavior on currentSpeed {
        NumberAnimation {
            duration: 100
        }
    }
}
