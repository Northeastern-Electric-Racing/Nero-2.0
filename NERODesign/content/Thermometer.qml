import QtQuick 6.5
import QtQuick.Controls 6.5
import QtQuick.Layouts

Item {
    id: thermometer
    property bool regen: false
    property int value: 0
    property int maxValue: 65
    property int minValue: -15
    property string color
    height: 500
    width: 200

    property real intersectPoint: 0.2
    property real totalFillPercentage: (Math.max(0, value - minValue) / (maxValue - minValue))
    property real circleFillPercentage: Math.min(1, totalFillPercentage / intersectPoint)
    property real tubeFillPercentage: Math.max(0, (totalFillPercentage - intersectPoint) / (1 - intersectPoint))
    property bool increasing: value > previousValue
    property int previousValue: value

    onValueChanged: {
        increasing = value > previousValue;
        previousValue = value;
    }

    Rectangle {
        visible: thermometer.regen
        id: lightningBackground
        width: parent.width / 3
        height: parent.height / 3
        color: "black"
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        anchors.verticalCenterOffset: -parent.height / 20
        radius: 0
        rotation: -45
        anchors.centerIn: parent
    }

    Rectangle {
        id: topSemiCircle
        anchors.horizontalCenter: outerRectangle.horizontalCenter
        anchors.top: parent.top
        width: parent.width / 3
        height: parent.width / 5
        radius: parent.width / 10
        color: thermometer.color
    }

    Rectangle {
        id: bottomCircle
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.baselineOffset: outerRectangle.bottom - 400
        y: parent.height / 2
        width: thermometer.width / 2
        height: thermometer.width / 2
        radius: width / 2
        color: "black"
        border.width: width * 0.15
        border.color: thermometer.color
        clip: true

        Rectangle {
            id: innerFillCircle
            width: parent.width - parent.border.width * 2
            height: parent.height - parent.border.width * 2
            radius: (parent.width - parent.border.width * 2) / 2
            anchors.centerIn: parent
            color: "black"
            clip: true

            Rectangle {
                id: gradientFill
                width: parent.width
                height: innerFillCircle.height * circleFillPercentage
                anchors.bottom: innerFillCircle.bottom
                radius: innerFillCircle.radius
                gradient: Gradient {
                    GradientStop { position: 0.0; color: thermometer.color }
                    GradientStop { position: 0.5; color: Qt.darker(thermometer.color, 1.5) }
                    GradientStop { position: 1.0; color: Qt.darker(thermometer.color, 2.5) }
                }
            }
        }
    }

    Rectangle {
        id: outerRectangle
        anchors.top: topSemiCircle.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter
        width: parent.width / 3
        height: parent.height * .5
        color: thermometer.color

        Rectangle {
            id: innerRectangle
            width: parent.width / 1.75
            height: parent.height
            anchors {
                centerIn: parent
                verticalCenterOffset: topSemiCircle.height / 100
            }
            color: "black"
            clip: true
        }

        Rectangle {
            id: thermometerFill
            width: innerRectangle.width
            height: innerRectangle.height
            anchors {
                centerIn: innerRectangle
            }
            clip: true
            color: "black"

            Rectangle {
                id: fillElement
                width: parent.width
                height: parent.height * tubeFillPercentage
                anchors.bottom: parent.bottom
                color: thermometer.color

                Behavior on height {
                    NumberAnimation {
                        duration: 1000
                        easing.type: Easing.OutQuad
                        running: thermometer.totalFillPercentage > thermometer.intersectPoint && increasing
                    }
                }

                Behavior on height {
                    NumberAnimation {
                        duration: 1000
                        easing.type: Easing.InOutQuad
                        running: thermometer.totalFillPercentage <= thermometer.intersectPoint && !increasing
                    }
                }
            }
        }
    }

    Lightning {
        id: mainLightning
        visible: thermometer.regen
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        anchors.verticalCenterOffset: -parent.height / 20
        dimension: parent.height / 2.25
    }
}
