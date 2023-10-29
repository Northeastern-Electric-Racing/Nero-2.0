import QtQuick 6.5
import QtQuick.Controls 6.5
import QtQuick.Layouts

Item {
    id: thermometer
    property int value: 0
    property int horizontalPadding: width / 20
    property int maxValue: 65
    property int minValue: -15
    property string color: value > maxValue - ((Math.abs(maxValue) + Math.abs(minValue)) / 5) ?
                               "red" : value > maxValue - (((Math.abs(maxValue) + Math.abs(minValue)) / 5) * 2) ?
                                   "orange" : value > maxValue - (((Math.abs(maxValue) + Math.abs(minValue)) / 5) * 3) ?
                                       "green" : value > maxValue - (((Math.abs(maxValue) + Math.abs(minValue)) / 5) * 4) ?
                                           "blue" : "purple"
    width: 50
    height: width


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
                id: fillBottomCircle
                anchors.centerIn: parent
                width: parent.width * 0.9
                height: parent.width * 0.9
                radius: parent.width * 0.9
                color: thermometer.color
            }
        }

        Rectangle {
            id: fillRectangle
            anchors.centerIn: parent
            width: parent.width - (thermometer.horizontalPadding * 2)
            height: parent.height * 0.9
            color: thermometer.color
        }
    }
}
