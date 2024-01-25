import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.0

Item {
    id: detailDisplay
    width: 200
    height: 50
    property string shutdownFlowTask: "Task" //controller.task
    property string status: "null" //controller.status
    property bool ok: false //controller.ok
    property bool clear: false //controller.clear
    property bool taskHovered: false
    property bool highlighted: false

    RowLayout {
        id: rowLayout
        anchors.fill: parent

        Rectangle {
            id: task
            color: {
                if (taskHovered && highlighted) return "darkgrey";
                else if (taskHovered) return "lightgrey";
                else if (highlighted) return "lightgrey";
                else return "transparent";
            }
            Layout.preferredWidth: parent.width / 2
            Layout.fillHeight: true
            radius: 10

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    displayOffScreenDescriptionModal();
                }

                onEntered: {
                    taskHovered = true;
                }

                onExited: {
                    taskHovered = false;
                }
            }

            LabelText {
                text: shutdownFlowTask
                anchors.centerIn: parent
            }
        }

        Rectangle {
            id: flag
            color: clear ? "transparent" : (ok === true ? "green" : (ok === false ? "red" : "transparent"))
            Layout.preferredWidth: parent.width / 2 - 10
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
