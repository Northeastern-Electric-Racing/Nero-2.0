import QtQuick 6.5
import QtQuick.Controls 6.5
import QtQuick.Layouts
import NERO

Rectangle {
    id: thermometer
    property bool regen: false
    property int value: 0
    property int maxValue: 65
    property int minValue: -15
    property string fillColor: regen ? Theme.redThermoStatus : value > maxValue - ((Math.abs(maxValue) + Math.abs(
                                                                        minValue)) / 5) ? Theme.redThermoStatus : value > maxValue - (((Math.abs(maxValue) + Math.abs(minValue)) / 5) * 2) ? Theme.orangeThermoStatus : value > maxValue - (((Math.abs(maxValue) + Math.abs(minValue)) / 5) * 3) ? Theme.yellowThermoStatus : value > maxValue - (((Math.abs(maxValue) + Math.abs(minValue)) / 5) * 4) ? Theme.blueThermoStatus : Theme.purpleThermoStatus
    height: 500
    width: height / 2.233
    color: "transparent"

    property int thermometerWidth: thermometer.height / 2.233
    property int horizontalPadding: thermometer.thermometerWidth / 10
    property int outerRectangleWidth: thermometer.thermometerWidth / 1.53
    property int outerRectangleHeight: thermometer.height / 1.84

    Rectangle {
        visible: thermometer.regen
        id: lightningBackground
        width: thermometer.thermometerWidth / 3
        height: parent.height / 4
        color: thermometer.fillColor
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        anchors.verticalCenterOffset: -parent.height / 20
        radius: 20
        rotation: -45
        anchors.centerIn: parent
    }

    Rectangle {
        id: topSemiCircle
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        width: thermometer.outerRectangleWidth
        height: parent.height / 12.5
        radius: parent.height / 12.5
        color: thermometer.fillColor
    }

    Rectangle {
        id: outerRectangle
        anchors.top: topSemiCircle.verticalCenter
        anchors.bottom: bottomOuterCircle.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter
        width: thermometer.outerRectangleWidth
        height: thermometer.outerRectangleHeight
        color: thermometer.fillColor
    }

    Rectangle {
        id: bottomOuterCircle
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: thermometer.bottom
        width: thermometer.thermometerWidth
        height: thermometer.thermometerWidth
        radius: thermometer.thermometerWidth
        color: thermometer.fillColor
    }

    Rectangle {
        id: fillBottomCircle
        anchors.centerIn: bottomOuterCircle
        width: bottomOuterCircle.width * 0.8
        height: bottomOuterCircle.height * 0.8
        radius: bottomOuterCircle.radius
        z: 1

        gradient: Gradient {
            stops: [
                GradientStop {
                    position: 1.0
                    color: Theme.fillGradientStop
                },
                GradientStop {
                    position: -0.8
                    color: thermometer.fillColor
                }
            ]
        }
    }

    Rectangle {
        id: fillRectangle
        anchors.fill: outerRectangle

        anchors.leftMargin: thermometer.horizontalPadding
        anchors.rightMargin: thermometer.horizontalPadding
        anchors.bottom: fillBottomCircle.top
        anchors.topMargin: thermometer.height / 5
        gradient: Gradient {
            stops: [
                GradientStop {
                    position: 1.5
                    color: Theme.fillGradientStop
                },
                GradientStop {
                    position: 0.0
                    color: thermometer.fillColor
                }
            ]
        }
        z: 0
    }

    Rectangle {
        id: blackFillRectangle
        anchors {
            top: outerRectangle.top
            left: fillRectangle.left
            right: fillRectangle.right
            bottom: fillRectangle.top
        }
        color: Theme.background
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
