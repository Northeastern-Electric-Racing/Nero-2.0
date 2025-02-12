import QtQuick 2.15
import QtQuick.Controls 2.15

Rectangle {
    id: snakeGame

    anchors.fill: parent
    focus: snakeGame.isFocused
    visible: true
    property bool directionCooldown: false
    property bool isFocused: false

    width: 800
    height: 480
    color: "black"

    property int tileSize: 20
    property int gridWidth: Math.floor(width / tileSize)
    property int gridHeight: Math.floor(height / tileSize)

    property var snakeBody: []

    QtObject {
        id: food
        property int x: Math.floor(gridWidth / 3 * 2)
        property int y: Math.floor(gridHeight / 2)
    }

    property int direction: 1  // 0: Up, 1: Right, 2: Down, 3: Left
    property bool gameOver: false
    property int score: 0

    Component.onCompleted: startGame()

    function startGame() {
        console.log(direction)
        let centerX = Math.floor(gridWidth / 2)
        let centerY = Math.floor(gridHeight / 2)
        snakeBody = [
            { x: centerX,     y: centerY },
            { x: centerX - 1, y: centerY },
            { x: centerX - 2, y: centerY }
        ]
        direction = 1
        gameOver = false
        score = 0
        food.x = Math.floor(gridWidth / 3 * 2)
        food.y = centerY
        updateSnakeModel()
        gameTimer.start()
    }

    function placeFood() {
        var x, y
        do {
            x = Math.floor(Math.random() * gridWidth)
            y = Math.floor(Math.random() * gridHeight)
        } while (isSnakePosition(x, y))
        food.x = x
        food.y = y
    }

    function isSnakePosition(x, y) {
        for (let i = 0; i < snakeBody.length; i++) {
            if (snakeBody[i].x === x && snakeBody[i].y === y) {
                return true
            }
        }
        return false
    }

    function checkCollision(x, y) {
        if (x < 0 || x >= gridWidth || y < 0 || y >= gridHeight) {
            return true
        }
        for (let i = 1; i < snakeBody.length; i++) {
            if (snakeBody[i].x === x && snakeBody[i].y === y) {
                return true
            }
        }
        return false
    }

    function updateGame() {
        if (gameOver) {
            gameTimer.stop()
            snakeController.saveScore(score)
            return
        }

        let head = { x: snakeBody[0].x, y: snakeBody[0].y }

        switch (direction) {
            case 0: // Up
                head.y -= 1
                break
            case 1: // Right
                head.x += 1
                break
            case 2: // Down
                head.y += 1
                break
            case 3: // Left
                head.x -= 1
                break
        }

        if (checkCollision(head.x, head.y)) {
            gameOver = true
            return
        }

        snakeBody.unshift(head)

        if (head.x === food.x && head.y === food.y) {
            score += 1
            placeFood()
        } else {
            snakeBody.pop()
        }

        updateSnakeModel()
    }

    function updateSnakeModel() {
        snakeModel.clear()
        for (let segment of snakeBody) {
            snakeModel.append({ "x": segment.x, "y": segment.y })
        }
    }

    Timer {
        id: gameTimer
        interval: 100
        repeat: true
        running: false
        onTriggered: updateGame()
    }

    Timer {
        id: cooldownTimer
        interval: 75 // Adjust this value for the desired cooldown period
        repeat: false
        onTriggered: directionCooldown = false
    }

    Keys.onPressed: {
        if (directionCooldown) return; // Ignore input during cooldown

        if ((event.key === Qt.Key_Up && direction !== 2) ||
            (event.key === Qt.Key_Right && direction !== 3) ||
            (event.key === Qt.Key_Down && direction !== 0) ||
            (event.key === Qt.Key_Left && direction !== 1)) {
            directionCooldown = true;
            cooldownTimer.start(); // Start the cooldown timer
            switch (event.key) {
                case Qt.Key_Up: direction = 0; break;
                case Qt.Key_Right: direction = 1; break;
                case Qt.Key_Down: direction = 2; break;
                case Qt.Key_Left: direction = 3; break;
            }
        }

        if (event.key === Qt.Key_Return && gameOver) {
            startGame();
        }
    }



    ListModel {
        id: snakeModel
    }

    Repeater {
        model: snakeModel
        delegate: SnakeBody {
            x: model.x * tileSize
            y: model.y * tileSize
            dimension: tileSize
        }
    }

    SnakeFood {
        x: food.x * tileSize
        y: food.y * tileSize
        dimension: tileSize
    }

    Text {
        id: gameOverText
        anchors.centerIn: parent
        text: gameOver ? "Game Over\nPress Enter to Restart" : ""
        color: "white"
        font.pixelSize: 24
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        visible: gameOver
    }

    Text {
        id: scoreText
        text: "Score: " + score
        font.pixelSize: 24
        color: "white"
        anchors.top: parent.top
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.topMargin: 10
    }

    Connections {
        target: snakeController
        onDirectionChanged: {
            direction = newDirection
            console.log("Direction updated to:", direction)
        }
    }
}
