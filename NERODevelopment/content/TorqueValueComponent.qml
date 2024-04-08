import QtQuick 2.15
import QtQuick.Layouts

Rectangle {
    id: torqueComponent
    property int torqueValue: 0
    property string title: "TORQUE LIMIT"
    property int horizontalIconSpacing: width * 0.1
    property int labelVerticalSpacing: height * 0.1
    property bool upSelected: false
    property bool downSelected: false

    color: 'transparent'
    height: 100
    width: 100

    Rectangle {
        id: torqueRow
        anchors.top: parent.top
        anchors.horizontalCenter: parent.horizontalCenter
        height: torqueComponent.height * 0.6
        color: 'transparent'
        anchors.topMargin: torqueComponent.labelVerticalSpacing

        TorqueAdj {
            id: torqueAdj
            height: parent.height
            width: parent.height / 2
            anchors.top: parent.top
            anchors.right: parent.horizontalCenter
            anchors.rightMargin: torqueComponent.horizontalIconSpacing
            upSelected: torqueComponent.upSelected
            downSelected: torqueComponent.downSelected
        }

        ValueText {
            text: torqueComponent.torqueValue
            font.pixelSize: 0.5 * torqueComponent.width
            anchors.left: torqueAdj.right
            anchors.leftMargin: torqueComponent.horizontalIconSpacing
            anchors.top: parent.top
            anchors.bottom: parent.bottom
        }
    }

    LabelText {
        anchors.top: torqueRow.bottom
        anchors.bottom: parent.bottom
        anchors.topMargin: torqueComponent.labelVerticalSpacing
        anchors.horizontalCenter: parent.horizontalCenter
        text: torqueComponent.title
        font.pixelSize: 0.15 * parent.width
    }
}
