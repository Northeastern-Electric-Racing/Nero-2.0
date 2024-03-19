import QtQuick 2.15
import QtQuick.Layouts

Rectangle {
    id: thermometerComponent
    property int thermometerValue: 0
    property string title: "MOTOR TEMP"
    property int horizontalIconSpacing: -width * 0.1
    property int labelVerticalSpacing: height * 0.1

    anchors.fill: parent
    color: 'black'

    Rectangle {
        id: tempColumn
        anchors.fill: parent
        color: 'transparent'

        Rectangle {
            id: thermRow
            anchors.top: parent.top
            anchors.horizontalCenter: parent.horizontalCenter
            height: thermometerComponent.height * 0.6
            color: 'transparent'

            Thermometer {
                id: thermometer
                value: thermometerValue
                height: parent.height
                anchors.top: parent.top
                anchors.topMargin: labelVerticalSpacing
                anchors.right: parent.horizontalCenter
                width: thermometerComponent.width / 2
            }

            ValueText {
                text: thermometerValue
                font.pixelSize: 0.5 * thermometerComponent.width
                anchors.left: thermometer.right
                anchors.top: parent.top
                anchors.topMargin: labelVerticalSpacing
            }
        }

        LabelText {
            anchors.top: thermRow.bottom
            anchors.bottom: parent.bottom
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.topMargin: thermometerComponent.labelVerticalSpacing
            text: title
            font.pixelSize: 0.15 * parent.width
        }
    }
}
