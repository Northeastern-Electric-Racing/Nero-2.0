import QtQuick 2.15
import QtQuick.Controls 2.15

Rectangle {
    id: snake

    width: 800
    height: 480
    color: "black"

    property int minX: 200
    property int maxX: 600
    property int minY: 40
    property int maxY: 440

    property var snakeBody: [{"x": 300, "y": 300}, {"x": 280, "y": 300}, {"x": 260, "y": 300}]
    property int segmentSize: 20
    property string direction: "right"

    property int score: 0

    Rectangle {
        width: 400
        height: 400
        anchors.centerIn: parent
        border.color: "white"
        border.width: 2
        color: "transparent"
    }

    Text {
        id: scoreText
        text: "Score: " + snake.score
        font.pixelSize: 24
        color: "white"
        anchors.top: parent.top
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.topMargin: 10
    }
}
