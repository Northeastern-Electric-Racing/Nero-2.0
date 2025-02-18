import QtQuick 2.15
import QtQuick.Layouts

Rectangle {
    id: batteryComponent
    property int value: 0
    property string title: "PACK SOC"
    property string textColor: "#4DA6FF"
    property string componentColor: "#00FF00"
    height: 0
    width: 100
    color: "transparent"

    Rectangle {
        id: background
        anchors.fill: parent
        color: "#151515"
        radius: 20
    }

    LabelText {
        id: titleText
        anchors {
            top: parent.top
            horizontalCenter: parent.horizontalCenter
            margins: 10
        }
        text: title
        color: textColor
        font.bold: true
        font.pixelSize: 24
    }

    Rectangle {
        id: contentRow
        anchors {
            fill: parent
            margins: 10
        }
        color: "transparent"

        Battery {
            id: battery
            value: batteryComponent.value
            color: componentColor
            height: parent.height * .65
            width: height * 1
            anchors {
                horizontalCenter: parent.horizontalCenter
                horizontalCenterOffset: -parent.width * 0.3
                verticalCenter: parent.verticalCenter
                verticalCenterOffset: parent.height * 0.1
            }
        }

        // Changed to use textColor
        ValueText {
            id: valueNumber
            text: batteryComponent.value
            font.pixelSize: 62
            color: white  // Now using textColor instead of "white"
            font.bold: true
            anchors {
                horizontalCenter: parent.horizontalCenter
                horizontalCenterOffset: parent.width * 0.1
                verticalCenter: parent.verticalCenter
                verticalCenterOffset: parent.height * 0.1
            }
        }

        // Changed to use textColor
        Text {
            text: "%"
            color: '#777777'  // Now using textColor instead of "white"
            font.pixelSize: 35
            font.bold: true
            anchors {
                left: valueNumber.right
                top: valueNumber.top
                topMargin: -12
            }
        }
    }
}
