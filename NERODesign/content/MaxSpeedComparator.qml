import QtQuick 2.15
import QtQuick.Controls 2.15

Rectangle {
    id: spedometer
    property int dimension: 100
    property int maxSpeed: 100
    property int lowestSpeed: 0
    property int currentSpeed: 30 //change to 0

    width: dimension
    height: dimension * 3
    radius: 5
    color: "black"
    border.color: "white"

    Text {
        id: maxSpeed
        text: spedometer.maxSpeed
        color: "white"
        x: -(width + dimension/20)
    }

    Text {
        id: lowestSpeed
        text: spedometer.lowestSpeed
        color: "white"
        x: -(width + dimension/20)
        y: spedometer.height - height
    }

    Text {
        id: topSpeed
        text: qsTr("TOP SPEED")
        color: "white"
        x: -(width + dimension/20)
        y: (spedometer.height/2)
        Rectangle {
            id: topSpeedBar
            x: spedometer.width - dimension/6
            width: spedometer.width - dimension/6
            height: 2
            color: "white"
        }

        Text {
            id: topSpeedNumber
            text: 100 + qsTr("MPH")
            color: "white"
            y: -height
            x: (topSpeed.width - width)
        }
    }

    Rectangle {
        id: bar
        x: spedometer.width - dimension/1.09
        y: (spedometer.height - height) - 5 //change
        width: topSpeedBar.width
        height: spedometer.currentSpeed
        color: "green"
        transformOrigin: Item.BottomLeft
        Behavior on rotation {
            SpringAnimation {
                spring: 1.4
                damping: .15
            }
          }
    }


    function setCurrentSpeed(speed) {
        spedometer.currentSpeed = speed
    }

    function setTopSpeed(top_speed) {
        topSpeedNumber.text = top_speed + "MPH"
        topSpeed.y = top_speed
    }

}
