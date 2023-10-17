import QtQuick 6.5
import QtQuick.Controls 6.5
import NERO
import QtQuick.Layouts

Item {
    property int width: 1920
    property int height: 1080
    Rectangle {
        id: rectangle
        x: parent.width / 2
        y: parent.height / 4
        width: 70
        height: 200
        color: "white"

        Rectangle {
            id: topSemiCircle
            x: 0
            y: -40
            width: 70
            height: 70
            radius: 70
            color: "white"
        }

        Rectangle {
            id: bottomOuterCircle
            x: -23.5
            y: 170
            width: 120
            height: 120
            radius: 120
            color: "white"

            Rectangle {
                id: greenRectangle
                x: 33.5
                y: -150
                width: 50
                height: 210
                color: "#55ff00"
            }

            Rectangle {
                id: bottomGreenCircle
                x: 10
                y: 10
                width: 100
                height: 100
                radius: 100
                color: "#55ff00"
            }
        }
    }
}
