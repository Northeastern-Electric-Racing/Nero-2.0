import QtQuick 2.15

import QtQuick 2.9
import QtQuick.Window 2.2

Item {
    id: flappyBird
    anchors.fill: parent
    focus: true
    visible: true
    width: 800
    height: 480

    property int xBallValue: 150
    property int yBallValue: 250

    property int xWall1: 200
    property int xWall2: 400
    property int xWall3: 600
    property string pipeSrc: "qrc:/content/images/pipe.png"
    property string backgroundSrc: "qrc:/content/images/bg.png"

    Timer {
        interval: 50
        running: true
        repeat: true
        onTriggered: {
            time.text = Date().toString()
            if (yBallValue > parent.height) {
                console.log("game over")
            }
            if (xWall1 < 0) {
                xWall1 = parent.width
            }
            if (xWall2 < 0) {
                xWall2 = parent.width
            }
            if (xWall3 < 0) {
                xWall3 = parent.width
            }

            yBallValue += 2
            xWall1 -= 8
            xWall2 -= 8
            xWall3 -= 8
        }
    }

    Image {
        id: background
        width: parent.width
        height: parent.height
        source: backgroundSrc
    }

    Image {
        id: wall1
        x: xWall1 + 20
        y: -150
        source: pipeSrc
        rotation: 180
    }

    Image {
        id: wall11
        x: xWall1
        y: 300
        source: pipeSrc
    }
    Image {
        id: wall21
        x: xWall2 + 20
        y: -140
        source: pipeSrc
        rotation: 180
    }

    Image {
        id: wall22
        x: xWall2
        y: 300
        source: pipeSrc
    }

    Image {
        id: wall31
        x: xWall3 + 20
        y: -140
        source: pipeSrc
        rotation: 180
    }

    Image {
        id: wall32
        x: xWall3
        y: 300
        source: pipeSrc
    }
    Keys.onSpacePressed: {
        yBallValue -= 20
    }

    Text {
        id: time
    }

    Rectangle {
        id: ball
        x: xBallValue
        y: yBallValue
        width: 50
        height: 50
        radius: 25
        color: "red"
    }
}
