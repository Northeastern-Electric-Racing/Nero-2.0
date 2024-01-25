import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.0

Item {
    id: detailDisplay
    width: 200
    height: 50
    property string shutdownFlowTask: "Task"
    property string status: "null"
    property bool ok: false
    property bool clear: true

    RowLayout {
        id: rowLayout
        anchors.fill: parent

        Rectangle {
            id: task
            color: "transparent" // Set the background color to transparent
            Layout.preferredWidth: parent.width/2
            Layout.fillHeight: true
            LabelText {
                text: shutdownFlowTask
                anchors.centerIn: parent
            }
        }

        Rectangle {
            id: flag
            width: 75
            height: 40
            color: clear ? "transparent" : (ok === true ? "green" : (ok === false ? "red" : "transparent"))
            Layout.preferredWidth: parent.width/2
            Layout.fillHeight: true
            radius: 10
            border.color: "#ffffff"
            LabelText {
                color: "#000000"
                text: status
                anchors.centerIn: parent
            }
        }
    }
}
