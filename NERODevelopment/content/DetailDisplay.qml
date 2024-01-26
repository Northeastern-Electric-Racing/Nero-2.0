import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.0
RowLayout {
    id: rowLayout
    anchors.fill: parent
    property string shutdownFlowTask: "Task" //controller.task
    property string status: "null" //controller.status
    property bool ok: false //controller.ok
    property bool clear: false //controller.clear
    property bool taskHovered: true
    property bool highlight: false //controller.highlight
    width: 200
    height:50


    Button {
        id: dashId
        width: parent.width / 2
        height: parent.height
        implicitWidth: parent.width / 2
        implicitHeight: parent.height
        flat: true
        text: shutdownFlowTask
        highlighted: true

        property color colorNormal:  (highlight) ? "lightgrey" : "transparent"
        property color colorHovered: (highlight) ? "darkgrey" : "lightgrey"
        property color colorClicked: "transparent"


        background: Rectangle{
            id: bgColor
            radius: 10
            color: internal.hoverColor
        }
        contentItem: Item {
            id: buttonItem
            visible: true
            Text {
                id: buttonText
                text: dashId.text
                anchors.centerIn: parent
                color: "white"
            }
        }

        QtObject{
         id: internal

         property var hoverColor: if(dashId.down){
                                      dashId.down ? colorClicked : colorNormal
                                  }else{
                                      dashId.hovered ? colorHovered : colorNormal
                                  }

        }
    }


    Rectangle {
        id: flag
        color: clear ? "transparent" : (ok === true ? "green" : (ok === false ? "red" : "transparent"))
        Layout.preferredWidth: parent.width / 2 - 5
        Layout.fillHeight: true
        radius: 10
        border.color: "white"
        LabelText {
            visible: !clear
            color: "black"
            text: status
            anchors.centerIn: parent
        }
    }
}



