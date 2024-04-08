import QtQuick 2.15
import QtQuick.Layouts

Rectangle {
    id: batteryComponent
    property int batteryValue: 0
    property string title: "MOTOR TEMP"
    property int horizontalIconSpacing: width * 0.1
    property int labelVerticalSpacing: height * 0.1

    height: 100
    width: 100
    color: 'transparent'

    Rectangle {
        id: batteryRow
        anchors.top: parent.top
        anchors.horizontalCenter: parent.horizontalCenter
        height: batteryComponent.height * 0.8
        color: 'transparent'

        Battery {
            id: battery
            value: batteryValue
            height: parent.height
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.topMargin: batteryComponent.labelVerticalSpacing
            anchors.right: parent.horizontalCenter
            anchors.rightMargin: batteryComponent.horizontalIconSpacing

            width: batteryComponent.width / 2 - batteryComponent.horizontalIconSpacing * 2
        }

        ValueText {
            text: batteryValue
            font.pixelSize: 0.6 * batteryComponent.height
            anchors.left: battery.right
            anchors.leftMargin: batteryComponent.horizontalIconSpacing
            anchors.top: parent.top
            anchors.topMargin: batteryComponent.labelVerticalSpacing
            anchors.bottom: parent.bottom
        }
    }

    LabelText {
        anchors.top: batteryRow.bottom
        anchors.topMargin: batteryComponent.labelVerticalSpacing
        anchors.bottom: parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        text: title
        font.pixelSize: 0.15 * parent.width
    }
}
