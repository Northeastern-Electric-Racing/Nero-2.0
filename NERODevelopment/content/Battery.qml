import QtQuick 2.15
import QtQuick.Controls 2.15

Rectangle {
    id: battery
    property int value: 100
    property int maxValue: 100
    property int fillMargin: height / 20

    property string fillColor: battery.value > 70 ? "#55FF00" : battery.value
                                                    > 40 ? "orange" : "red"
    width: height / 2
    color: 'transparent'

    Rectangle {
        id: topOutlet
        anchors.horizontalCenter: parent.horizontalCenter
        width: parent.width / 2
        height: parent.height / 10
        color: battery.fillColor
        anchors.top: parent.top
    }

    Rectangle {
        id: mainContainer
        color: battery.fillColor
        anchors {
            top: topOutlet.bottom
            right: parent.right
            bottom: parent.bottom
            left: parent.left
        }

        Rectangle {
            id: blackFill
            anchors {
                fill: parent
                margins: battery.fillMargin
            }
            color: 'black'
            z: 0
        }

        Rectangle {
            id: fillContainer
            height: (parent.height - (battery.fillMargin * 2)) * (battery.value / battery.maxValue)
            gradient: Gradient {
                stops: [
                    GradientStop {
                        position: 0.0
                        color: battery.fillColor
                    },
                    GradientStop {
                        position: 1.0
                        color: 'black'
                    }
                ]
            }
            anchors {
                bottom: parent.bottom
                left: parent.left
                right: parent.right
                bottomMargin: battery.fillMargin
                leftMargin: battery.fillMargin
                rightMargin: battery.fillMargin
            }
            z: 1
        }
    }
}
