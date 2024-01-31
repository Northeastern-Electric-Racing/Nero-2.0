import QtQuick 2.15
import QtQuick.Controls 2.15

Item {
    id: detailDisplay
    property string shutdownFlowTask: "Task"
    property string status: "null"
    property bool ok: true
    property bool clear: false
    property bool highlight: true
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

            LabelText {
                id: taskText
                color: "white"
                text: detailDisplay.shutdownFlowTask
                anchors.centerIn: parent
            }
        }

        Rectangle {
            id: statusDisplay
            color: clear ? "transparent" : (ok === true ? "#14FF00" : (ok === false ? "#FF0000" : "transparent"))
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
