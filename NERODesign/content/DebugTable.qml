import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Particles

Item {
    id: debugTable
    width: 800
    height: 480

    property var topicList: ["topic1", "topic2", "topic3"]

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
                width: parent.width
                height: parent.height
                spacing: 10

                Column {
                    width: parent.width / 2
                    height: parent.height
                    Repeater {
                        model: debugTable.topicList.length // Use the length of the topicList as the model count
                        delegate:
                            Rectangle {
                                width: parent.width / 2 // Divide by 2 since we're in the first column
                                height: parent.height * 0.07 // Height same as parent
                                color: "black"

                                LabelText {
                                    color: "white"
                                    text: debugTable.topicList[index] // Set text to the current topic
                                }
                            }
                    }
                }

                Column {
                    width: parent.width / 2
                    height: parent.height
                    Row {
                        width: parent.width
                        height: parent.height
                        Rectangle {
                            width: parent.width / 2 +10
                            height: parent.height
                            color: "black"

                            LabelText {
                                color: "white"
                                text: "NAME"
                            }
                        }

                        Rectangle {
                            width: parent.width / 2
                            height: parent.height
                            color: "black"

                            LabelText {
                                color: "white"
                                text: "VALUE"
                            }
                        }
                    }
                }
            }
        }
    }
}
