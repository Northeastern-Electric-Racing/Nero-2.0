import QtQuick 6.5
import QtQuick.Controls 6.5
import QtQuick.Layouts

Item {
    id: thermometer
    width: 1920

    Rectangle {
        id: rectangle
        anchors.centerIn: parent
        width: parent.width * (35/100)
        height: width * 2.85
        color: "white"

        Rectangle {
            id: topSemiCircle
            x: 0
            y: parent.height * (-18/100)
            width: parent.width
            height: parent.width
            radius: parent.width
            color: "white"
        }

        Rectangle {
            id: bottomOuterCircle
            x: -parent.width / (3 / 2)
            y: parent.height * 8 / 10
            width: parent.width + thermometer.width / 2
            height: parent.width + thermometer.width / 2
            radius: parent.width + thermometer.width / 2
            color: "white"



            Rectangle {
                id: bottomGreenCircle
                anchors.centerIn: parent
                width: parent.width * 0.9
                height: parent.width * 0.9
                radius: parent.width * 0.9
                color: "#55FF00"
            }
        }

        Rectangle {
            id: greenRectangle
            anchors.centerIn: parent
            width: parent.width * 0.8
            height: parent.height * 0.9
            color: "#55FF00"
        }
    }
}
