import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Particles
import Qt.labs.qmlmodels

Item {
    id: debugTable
    width: 800
    height: 480

    property variant dataModel: [
        ['topic1'],
        ['topic2'],
        ['topic3'],
    ]

    Rectangle {
        id: debugTableBackground
        width: parent.width
        height: parent.height
        color: "black"

        Column {
            id: container
            anchors.fill: parent
            anchors.margins: parent.width / 40

            Row {
                id: headers
                width: parent.width
                height: parent.height * 0.1
                anchors.rightMargin: 10
                spacing: 10

                Rectangle {
                    id: topic
                    width: parent.width / 2
                    height: parent.height
                    color: "black"

                    LabelText {
                        id: topicText
                        color: "white"
                        text: "TOPIC"
                    }
                }

                Rectangle {
                    id: name
                    width: parent.width / 4
                    height: parent.height
                    color: "black"

                    LabelText {
                        id: nameText
                        color: "white"
                        text: "NAME"
                    }
                }

                Rectangle {
                    id: value
                    width: parent.width / 4
                    height: parent.height
                    color: "black"

                    LabelText {
                        id: valueText
                        color: "white"
                        text: "VALUE"
                    }
                }
            }

            Row {
                id: topicContainer
                width: parent.width
                height: parent.height
                spacing: 10
                Column{
                    id: root
                    width: parent.width /2
                    height: parent.height

                    ListView { // data
                        anchors{fill: parent;}

                        model: debugTable.dataModel

                        delegate: Item { // row
                            width: root.width;  height: 50

                            property int     row:     index
                            property variant rowData: modelData

                            Row {
                                anchors.fill: parent

                                Repeater {
                                    model: rowData

                                    delegate: Rectangle { // cell
                                        width: root.width;  height: 50
                                        color:'black'
                                        Text {
                                            text: modelData
                                            anchors.verticalCenter: parent.verticalCenter
                                            font.pixelSize: 0.06 * root.width
                                            color:"white"
                                        }
                                    }
                                }
                            }
                        }
                    }
                }

                Column {
                    width: parent.width / 2
                    height:  parent.height
                    Row {
                        width: parent.width
                        height: parent.height

                    }
                }
            }
        }
    }
}
