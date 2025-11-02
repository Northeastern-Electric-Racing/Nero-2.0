import QtQuick 6.5
import QtQuick.Controls 6.5
import QtQuick.Layouts

Rectangle {
    id: thermometer
    property bool regen: false
    property int value: 0
    property int maxValue: 65
    property int minValue: -15

    property real percentage: (value - minValue) / (maxValue - minValue) //Converting value to percentage range between minval -> maxval

    property color fillColor: regen ? Qt.hsla(0, 1, 0.5, 1) :
                              Qt.hsla(
                                  percentage > 0.875 ? 0.0 : //Red
                                  percentage > 0.75 ? 0.08 + (0.0 - 0.08) * ((percentage - 0.75) / 0.125) : //Orange -> Red
                                  percentage > 0.5 ? 0.16 + (0.08 - 0.16) * ((percentage - 0.5) / 0.25) : // Yellow -> Orange
                                  percentage > 0.375 ? 0.33 + (0.16 - 0.33) * ((percentage - 0.375) / 0.125) : //Green -> Yellow
                                  percentage > 0.25 ? 0.55 + (0.33 - 0.55) * ((percentage - 0.25) / 0.125) : //Cyan -> Green
                                  percentage > 0.125 ? 0.67 + (0.55 - 0.67) * ((percentage - 0.125) / 0.125) : //Blue -> Cyan
                                  0.83 + (0.67 - 0.83) * (percentage / 0.125), //Purple -> Blue
                                  1, 0.5, 1 //Saturation, lightness, alpha
                              )

    height: 500
    width: height / 2.233
    color: 'transparent'

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
                    color: "black"
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
                    color: "black"
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
        color: "black"
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
