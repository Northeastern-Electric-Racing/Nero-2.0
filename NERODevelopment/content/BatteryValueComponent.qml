import QtQuick 2.15
import QtQuick.Layouts

Rectangle {
    id: batteryComponent
    property int batteryValue: 0
    property string title: "MOTOR TEMP"
    property int horizontalIconSpacing: -width * 0.1
    property int labelVerticalSpacing: height * 0.1

    anchors.fill: parent
    color: 'black'

    Rectangle {
        id: batteryColumn
        anchors.fill: parent
        color: 'transparent'

        Rectangle {
            id: batteryRow
            anchors.top: parent.top
            anchors.horizontalCenter: parent.horizontalCenter
            height: batteryComponent.height * 0.6
            color: 'transparent'

            Battery {
                id: battery
                value: batteryValue
                height: parent.height
                anchors.top: parent.top
                anchors.topMargin: labelVerticalSpacing
                anchors.right: parent.horizontalCenter
                anchors.rightMargin: batteryComponent.horizontalIconSpacing
                width: batteryComponent.width / 2
            }

            ValueText {
                text: batteryValue
                font.pixelSize: 0.5 * batteryComponent.width
                anchors.left: battery.right
                anchors.top: parent.top
                anchors.topMargin: batteryComponent.labelVerticalSpacing
            }
        }

        LabelText {
            anchors.top: batteryRow.bottom
            anchors.bottom: parent.bottom
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.topMargin: batteryComponent.labelVerticalSpacing
            text: title
            font.pixelSize: 0.15 * parent.width
        }
    }
}
