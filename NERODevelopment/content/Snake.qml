import QtQuick 2.15
import QtQuick.Window 2.15

Window {
    id: gameWindow
    width: 400
    height: 400
    visible: true
    title: "Snake Game"
    color: "black"

    property int tileSize: 20
    property int gridWidth: width / tileSize
    property int gridHeight: height / tileSize

    property var snake: []
    property var food: { 'x': 0, 'y': 0 }
    property string direction: "Right"
    property bool gameOver: false
    property int score: 0

    Component.onCompleted: startGame()

    function startGame() {
        snake = [{ x: Math.floor(gridWidth / 2), y: Math.floor(gridHeight / 2) }]
        direction = "Right"
        gameOver = false
        score = 0
        placeFood()
        gameTimer.start()
        updateSnakeModel()
    }

    function placeFood() {
        do {
            food.x = Math.floor(Math.random() * gridWidth)
            food.y = Math.floor(Math.random() * gridHeight)
        } while (isSnakePosition(food.x, food.y))
    }

    function isSnakePosition(x, y) {
        for (let i = 0; i < snake.length; i++) {
            if (snake[i].x === x && snake[i].y === y) {
                return true
            }
        }
        return false
    }

    function checkCollision(x, y) {
        if (x < 0 || x >= gridWidth || y < 0 || y >= gridHeight) {
            return true
        }
        for (let i = 1; i < snake.length; i++) {
            if (snake[i].x === x && snake[i].y === y) {
                return true
            }
        }
        return false
    }

    function updateGame() {
        if (gameOver) {
            gameTimer.stop()
            return
        }

        let head = { x: snake[0].x, y: snake[0].y }

        switch (direction) {
            case "Left":
                head.x -= 1
                break
            case "Right":
                head.x += 1
                break
            case "Up":
                head.y -= 1
                break
            case "Down":
                head.y += 1
                break
        }

        if (checkCollision(head.x, head.y)) {
            gameOver = true
            return
        }

        snake.unshift(head)

        if (head.x === food.x && head.y === food.y) {
            score += 1
            placeFood()
        } else {
            snake.pop()
        }

        updateSnakeModel()
    }

    function updateSnakeModel() {
        snakeModel.clear()
        for (let segment of snake) {
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

    Keys.onPressed: {
        if ((event.key === Qt.Key_Left || event.key === Qt.Key_A) && direction !== "Right") {
            direction = "Left"
        } else if ((event.key === Qt.Key_Right || event.key === Qt.Key_D) && direction !== "Left") {
            direction = "Right"
        } else if ((event.key === Qt.Key_Up || event.key === Qt.Key_W) && direction !== "Down") {
            direction = "Up"
        } else if ((event.key === Qt.Key_Down || event.key === Qt.Key_S) && direction !== "Up") {
            direction = "Down"
        } else if (event.key === Qt.Key_Space && gameOver) {
            startGame()
        }
    }

    ListModel {
        id: snakeModel
    }

    Repeater {
        model: snakeModel
        Rectangle {
            x: model.x * tileSize
            y: model.y * tileSize
            width: tileSize
            height: tileSize
            color: "lime"
        }
    }

    Rectangle {
        x: food.x * tileSize
        y: food.y * tileSize
        width: tileSize
        height: tileSize
        color: "red"
    }

    Text {
        id: gameOverText
        anchors.centerIn: parent
        text: gameOver ? "Game Over\nPress Space to Restart" : ""
        color: "white"
        font.pixelSize: 24
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        visible: gameOver
    }

    Text {
        id: scoreText
        text: "Score: " + score
        color: "white"
        font.pixelSize: 16
        anchors.top: parent.top
        anchors.topMargin: 10
        anchors.horizontalCenter: parent.horizontalCenter
    }
}
