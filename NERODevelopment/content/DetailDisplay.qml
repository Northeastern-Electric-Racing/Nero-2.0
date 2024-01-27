import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.0
import QtQuick.Dialogs 1.3

Item {
    id: detailDisplay
    property string shutdownFlowTask: "Task"
    property string status: "null"
    property bool ok: true
    property bool clear: false
    property bool highlight: true
    property bool displayModal: false

    width: 200
    height: 50

    Row {
        id: row
        width: parent.width
        height: parent.height
        anchors.centerIn: parent

        Rectangle {
            id: taskDisplay
            color: highlight ? "lightgrey" : "transparent"
            width: parent.width / 2
            height: parent.height
            radius: 10

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    detailDisplay.displayModal = true;
                }
            }

            LabelText {
                id: taskText
                color: "white"
                text: detailDisplay.shutdownFlowTask
                anchors.verticalCenterOffset: 3
                anchors.horizontalCenterOffset: -26
                anchors.centerIn: parent
            }
        }

        Rectangle {
            id: statusDisplay
            color: clear ? "transparent" : (ok === true ? "green" : (ok === false ? "red" : "transparent"))
            width: parent.width / 2
            height: parent.height
            radius: 10
            border.color: "white"

            LabelText {
                visible: !clear
                color: "black"
                text: detailDisplay.status
                anchors.centerIn: parent
            }
        }
    }
}
