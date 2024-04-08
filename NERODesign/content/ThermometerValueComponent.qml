import QtQuick 2.15
import QtQuick.Layouts

Rectangle {
    id: thermometerComponent
    property int thermometerValue: 0
    property string title: "MOTOR TEMP"
    property int horizontalIconSpacing: width * 0.1
    property int labelVerticalSpacing: height * 0.1
    property bool regen: false

    color: 'transparent'
    height: 100
    width: 100

    Rectangle {
        id: thermRow
        anchors.top: parent.top
        anchors.horizontalCenter: parent.horizontalCenter
        height: thermometerComponent.height * 0.6
        color: 'transparent'
        anchors.topMargin: thermometerComponent.labelVerticalSpacing

        Thermometer {
            id: thermometer
            value: thermometerValue
            height: parent.height
            width: parent.height
            anchors.top: parent.top
            anchors.right: parent.horizontalCenter
            regen: thermometerComponent.regen
        }

        ValueText {
            text: thermometerValue
            font.pixelSize: 0.5 * thermometerComponent.width
            anchors.left: thermometer.right
            anchors.top: parent.top
            anchors.topMargin: thermometerComponent.labelVerticalSpacing
            anchors.bottom: parent.bottom
        }
    }

    LabelText {
        anchors.top: thermRow.bottom
        anchors.topMargin: thermometerComponent.labelVerticalSpacing
        anchors.bottom: parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        text: title
        font.pixelSize: 0.15 * parent.width
    }
}
