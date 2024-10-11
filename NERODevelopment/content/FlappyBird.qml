import QtQuick 2.15

import QtQuick 2.9
import QtQuick.Window 2.2
import NERO

Item {
    id: flappyBird
    anchors.fill: parent
    focus: flappyBird.isFocused
    visible: true

    property bool isFocused: false

    property int xBallValue: 150
    property int yBallValue: 250

    property int xWall1: 200
    property int xWall2: 400
    property int xWall3: 600
    property string pipeSrc: "qrc:/content/images/pipe.png"
    property string backgroundSrc: "qrc:/content/images/bg.png"
    property string birdSrc: "qrc:/content/images/flappy-bird.png"
    property bool gameOver: true
    property int score: 0
    property bool jump: false
    property int birdHitBox: 30
    property int pipeWidth: 25
    property int pipeHeight: 300
    property int pipeOpeningheight: 150
    property double birdDrop: 1
    property double speed: 3
    property int frameRate: 25
    property bool didJump: flappyBirdController.didJump
    property string birdFrame1: "qrc:/content/images/yellowbird-downflap.png"
    property string birdFrame2: "qrc:/content/images/yellowbird-midflap.png"
    property string birdFrame3: "qrc:/content/images/yellowbird-upflap.png"

    property int currentBirdFrame: 0
    property bool startFalling: false

    property real birdRotation: 0


    onDidJumpChanged: {
        if (didJump) {
            if (flappyBird.gameOver) {
                flappyBird.gameOver = false
                flappyBird.score = 0
                flappyBird.yBallValue = 250
                flappyBird.xWall1 = 450
                flappyBird.xWall2 = 650
                flappyBird.xWall3 = 800
                flappyBird.speed = 3

                // Reset the bird's rotation and drop speed
                flappyBird.birdRotation = 0
                flappyBird.birdDrop = 1
                startFalling = false
                birdFallDelay.start()  // Start the fall timer to ensure bird starts falling even if no jump is pressed
                return
            }

            jumpAnimation.running = true
            birdDrop = 1
            startFalling = false
            birdFallDelay.restart()
        }
    }

    Timer {
        id: birdFallDelay
        interval: 150
        running: false
        repeat: false
        onTriggered: {
            startFalling = true  // Bird starts falling faster after delay
        }
    }


    Timer {
        function isGameOver() {
            return yBallValue > parent.height || yBallValue < 0 || (xBallValue + pipeWidth > xWall1 && xBallValue < xWall1 + pipeWidth && (yBallValue < wall1.y + pipeHeight || yBallValue + birdHitBox > wall11.y)) || (xBallValue + pipeWidth > xWall2 && xBallValue < xWall2 + pipeWidth && (yBallValue < wall21.y + pipeHeight || yBallValue + birdHitBox > wall22.y)) || (xBallValue + pipeWidth > xWall3 && xBallValue < xWall3 + pipeWidth && (yBallValue < wall31.y + pipeHeight || yBallValue + birdHitBox > wall32.y))
        }

        interval: flappyBird.frameRate
        running: flappyBird.focus
        repeat: true
        onTriggered: {
            if (flappyBird.gameOver) {
                flappyBirdController.saveScore(flappyBird.score)
                return
            }

            time.text = Date().toString()
            if (isGameOver()) {
                flappyBird.gameOver = true
            }

            if (xWall1 < 0) {
                xWall1 = parent.width
                wall1.y = -100 - Math.round(Math.random() * 100)
                wall11.y = wall1.y + pipeHeight + pipeOpeningheight
                flappyBird.score += 1
            }
            if (xWall2 < 0) {
                xWall2 = parent.width
                wall21.y = -100 - Math.round(Math.random() * 100)
                wall22.y = wall21.y + pipeHeight + pipeOpeningheight
                flappyBird.score += 1
            }
            if (xWall3 < 0) {
                xWall3 = parent.width
                wall31.y = -100 - Math.round(Math.random() * 100)
                wall32.y = wall31.y + pipeHeight + pipeOpeningheight
                flappyBird.speed += 0.2
                flappyBird.score += 1
            }

            flappyBird.speed += 0.005

            if (!jumpAnimation.running && startFalling) {
                yBallValue += flappyBird.birdDrop
                flappyBird.birdDrop += 0.3  // Increase the fall speed by incrementing faster
                flappyBird.birdRotation = Math.min(90, flappyBird.birdRotation + 5)  // Rotate the bird faster as it falls
            } else if (!jumpAnimation.running && !startFalling) {
                flappyBird.birdDrop = 1  // Slow initial fall after jump or at the start
                flappyBird.birdRotation = Math.min(45, flappyBird.birdRotation + 3)  // Slight rotation before full dive
            } else {
                flappyBird.birdRotation = -45  // Bird flaps up during jump
            }

            xWall1 -= flappyBird.speed
            xWall2 -= flappyBird.speed
            xWall3 -= flappyBird.speed
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
        x: xWall1
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
        x: xWall2
        y: -175
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
        x: xWall3
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
        flappyBirdController.enterButtonPressed()
        birdFlapAnimation.running = true
    }

    NumberAnimation on yBallValue {
        id: jumpAnimation
        to: yBallValue - 50
        duration: 100
    }

    Text {
        id: time
    }

    Image {
        id: ball
        x: xBallValue
        y: yBallValue
        width: implicitWidth
        height: implicitHeight
        source: {
                    if (currentBirdFrame === 0) return birdFrame1;
                    else if (currentBirdFrame === 1) return birdFrame2;
                    else return birdFrame3;
                }
        rotation: flappyBird.birdRotation
    }

    SequentialAnimation {
            id: birdFlapAnimation
            running: false
            loops: 1

            PropertyAnimation { target: flappyBird; property: "currentBirdFrame"; from: 2; to: 0; duration: 1000 }
            PropertyAnimation { target: flappyBird; property: "currentBirdFrame"; from: 0; to: 1; duration: 500 }
        }

    Text {
        id: gameOverText
        visible: flappyBird.gameOver
        anchors.centerIn: parent
        color: "Red"
        text: "Game Over (Hit Enter to play again)"
    }

    Text {
        id: scoreText
        text: "Score: " + flappyBird.score
        anchors.right: parent.right
        anchors.rightMargin: 8
        visible: true
    }
}
