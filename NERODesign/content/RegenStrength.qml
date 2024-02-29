import QtQuick 6.5
import QtQuick.Controls 6.5
import QtQuick.Layouts
import QtQuick.Shapes

Item {
    id: thermometer
    property int value: 0
    property int horizontalPadding: width / 20
    property int maxValue: 65
    property int minValue: -15
    property string color: "red"
    height: 300
    width: height

    Rectangle {
        id: lightningBackground
        width: parent.width / 3
        height: parent.height / 4
        color: "white"
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        anchors.verticalCenterOffset: - parent.height / 20
        radius: 20
        rotation: -45
        anchors.centerIn: parent
    }

    Rectangle {
        id: topSemiCircle
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        width: parent.width / 4
        height: parent.width / 4
        radius: parent.width / 4
        color: "white"
    }

    Rectangle {
        id: outerRectangle
        anchors.top: topSemiCircle.verticalCenter
        anchors.bottom: bottomOuterCircle.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter
        width: parent.width / 4
        height: parent.height / 2
        color: "white"
    }

    Rectangle {
        id: bottomOuterCircle
        anchors.horizontalCenter: parent.horizontalCenter
        y: parent.height / 2
        width: thermometer.width / 2
        height: thermometer.width / 2
        radius: thermometer.width / 2
        color: "white"

        Rectangle {
            id: fillBottomCircle
            anchors.centerIn: parent
            width: parent.width * 0.8
            height: parent.height * 0.8
            radius: parent.radius
            color: thermometer.color
        }
    }

    Rectangle {
        id: fillRectangle
        anchors.fill: outerRectangle

        anchors.leftMargin: thermometer.horizontalPadding
        anchors.rightMargin: thermometer.horizontalPadding
        color: thermometer.color
    }

    Lightning {
        id: mainLightning
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        anchors.verticalCenterOffset: - parent.height / 20
        dimension: parent.height / 2.25
    }
}
