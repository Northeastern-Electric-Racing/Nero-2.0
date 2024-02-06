import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Particles

Item {
    id: debugTable
    width: 800
    height: 480

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
                height: parent.height * 0.06
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
                    id: id
                    width: parent.width / 10
                    height: parent.height
                    color: "black"

                    LabelText {
                        id: idText
                        color: "white"
                        text: "ID"
                    }
                }

                Rectangle {
                    id: name
                    width: parent.width * 0.25
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
                    width: parent.width * 0.1
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
                width: parent.width
                height: parent.height * 0.06
                spacing: 10

                Rectangle {
                    width: parent.width / 2
                    height: parent.height
                    color: "black"
                    LabelText {
                        color: "white"
                        text: "Rectangle 1"
                    }
                }

                Rectangle {
                    width: parent.width / 2
                    height: parent.height
                    color: "black"
                    LabelText {
                        color: "white"
                        text: "Rectangle 2"
                    }
                }
            }
        }
    }
}
